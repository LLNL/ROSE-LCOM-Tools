import argparse
import concurrent.futures
import fnmatch
import os
import shutil
import subprocess
import time
import warnings

from threading import RLock
from excludes import EXCLUDES


class Config:
    """Configuration parameters.

    Configuration parameters that control several aspects of test invocation.
    Many of these can be changed via command line argument.
    """

    # The root of the project.
    PROJECT = os.environ["LCOM_HOME"]
    # The location of the DOT generator executable. Used to create ROSE DOT graphs.
    dotGenerator = os.path.join(os.environ["ROSE_REPO"],
                   "build_tree/exampleTranslators/DOTGenerator/dotGenerator")
    # An include string. Used to include necessary support files.
    # Currently only used to build ACATS tests.
    INCLUDE = "-I" + os.path.join(PROJECT, "testcases/acats/support")
    # The maximum number of jobs that this Python script will attempt to run
    # concurrently.
    JOB_LIMIT = os.cpu_count()  # Max 40 suggested on hudson-rose-65.
    # The parent directory of all tests. This is used to determine relative
    # paths to place tests in the output folder. It also serves as a working
    # folder. Several intermediate files are generated here.
    TEST_DIR = os.path.join(PROJECT, "testcases")
    # The folder to output tests.
    OUT_DIR = os.path.join(PROJECT, "output")
    # The location where the project is build.
    BUILD_DIR = os.path.join(PROJECT, "build")
    # The error log.
    # TODO: Unused.
    ERROR_LOG = os.path.join(OUT_DIR, "failures.log")
    # The directory within TEST_DIR to look for tests.
    # Modify this to limit which tests to run.
    DIR = os.path.join(TEST_DIR, "simple-cases")
    # [trace|debug|info|notice|warning|error|fatal]
    LOG_LEVEL = "trace"
    # [Default|Package|Function|Class|ProtectedObject|All]
    CLASS_TYPE = "All"
    # [LeftOnly|Full]
    DOT_BEHAVIOR = "Full"
    # Optional. Additional arguments passed into LCOM tool.
    ARGS = ""  # '--lcom:anonymous'


# Global task executor.
executor = concurrent.futures.ThreadPoolExecutor(max_workers=Config.JOB_LIMIT)
# Dependencies map. Allows specifying the name of the dependency to get the
# associated subprocess object.
deps = {}
# TODO: This task list may be somewhat brittle, as some tasks may be dependent
# on other tasks not in this list. It may be best to do dependency analysis
# before launching tasks to fill in this list, and only then start serving tasks
# out. It is adequate for the time being.
tasks_to_execute = []


class DeferredTask:
    """Stores task info to run later.

    A Deferred Task object stores all arguments needed to call a function and
    returns a future to get the result. This enables deferring task start as
    well as when the task result is read.
    """

    def __init__(self, func, *args, **kwargs):
        self.func = func
        self.args = args
        self.kwargs = kwargs
        self.future = None
        # self.lock = multiprocessing.managers.SyncManager.RLock()
        self.lock = RLock()

    def start(self):
        with self.lock:
            if self.future is not None:
                raise Exception("Task already started")
            self.future = executor.submit(self.func, *self.args, **self.kwargs)

    def result(self, optional=False):
        with self.lock:
            if self.future is not None:
                return self.future.result()
            if optional and self.func.__name__ not in tasks_to_execute:
                return None
            print(
                "Tried to get the result of a deferred task that hasn't started: \""
                + str(self.func.__name__)
                + '". Starting the task now.'
            )
            self.start()
            return self.future.result()


def execute(key):
    if key not in deps:
        raise Exception(
            'Tried to execute a deferred task, "'
            + str(key)
            + '" that is missing from the dependencies list. Please add it to the list before calling it.'
        )
    deps[key].start()
    return deps[key].future


def wait(key, optional=False):
    while key not in deps:
        warnings.warn(
            'task with key "'
            + key
            + '" was not properly deferred in advance. Waiting in hopes it shows up.'
        )
        time.sleep(1)
    return deps[key].result(optional)


def set_proc_limits():
    """Used by subprocesses to limit priority."""
    os.nice(19)


def run(args, out=None, cwd=Config.PROJECT):
    """Run a shell application."""
    stdout = None
    stderr = None
    if out:
        stdout = out
        stderr = stdout
    if not os.path.exists(cwd):
        os.makedirs(cwd, exist_ok=True)
    process = subprocess.Popen(
        args, stdout=stdout, stderr=stderr, preexec_fn=set_proc_limits, cwd=cwd
    )
    return process


def list_files(directory_path, extensions, use_exclusions=False):
    cmd = ["find", directory_path, "-type", "f", "("]
    if len(extensions) > 0:
        cmd += ["-iname", f"*.{extensions[0]}"]
    for ext in extensions[1:]:
        cmd += ["-o", "-iname", f"*.{ext}"]
    cmd += [")"]
    if use_exclusions:
        exclusions = [os.path.join(Config.TEST_DIR, e) for e in EXCLUDES]
        for exclude in exclusions:
            cmd += ["!", "-path", f"{exclude}"]
    cmd += ["-print0"]

    # DEBUG
    # print(" ".join(cmd))

    output = run(cmd, subprocess.PIPE)
    stdout, stderr = output.communicate()
    if stderr:
        print(f"Error: {stderr.decode()}")
        return []
    matches = stdout.decode(encoding="UTF-8", errors="backslashreplace").split("\0")[
        :-1
    ]

    # DEBUG
    # print(matches)

    return matches


def find_subfolders(path, prefix):
    subfolders = []
    for dirpath, dirnames, filenames in os.walk(path):
        for dirname in dirnames:
            if dirname.startswith(prefix):
                subfolders.append(os.path.join(dirpath, dirname))
    return subfolders


def build():
    print("Building")
    generate = run(["cmake", "-S", Config.PROJECT, "-B", Config.BUILD_DIR])
    generate.wait()
    if generate.returncode != 0:
        return generate.returncode
    build_exe = run(
        ["cmake", "--build", Config.BUILD_DIR, "--parallel", str(Config.JOB_LIMIT)]
    )
    build_exe.wait()
    return build_exe.returncode


def wipe_output():
    print("Deleting output folder")
    if os.path.exists(Config.OUT_DIR):
        shutil.rmtree(Config.OUT_DIR)


def make_dot_graphs():
    wait("wipe_output", optional=True)
    print("Generating ROSE DOT graphs")

    def make_dot_graph(file):
        print(
            "Generating ROSE DOT graph for "
            + file.encode("utf-8", "backslashreplace").decode()
        )
        SAVE_PATH = os.path.join(Config.OUT_DIR, os.path.relpath(file, Config.TEST_DIR))
        os.makedirs(os.path.dirname(SAVE_PATH), exist_ok=True)
        with open(SAVE_PATH + ".dot.log", "w") as f:
            # NOTE: cwd is changed to SAVE_PATH to ensure the generator outputs in the right place.
            return run(
                [Config.dotGenerator, file] + Config.INCLUDE.split(),
                f,
                os.path.dirname(SAVE_PATH),
            )

    src_files = list_files(Config.DIR, ["adb", "ads", "cpp"], use_exclusions=True)
    wait_list = []
    for file in src_files:
        wait_list.append(make_dot_graph(file))
    for task in wait_list:
        task.wait()


def gen_lcom():
    EXEC = os.path.join(Config.BUILD_DIR, "lcom.out")
    wait("build")
    wait("wipe_output", optional=True)
    print("Computing LCOM")

    def lcom(file):
        print(
            "Generating LCOM metrics for "
            + file.encode("utf-8", "backslashreplace").decode()
        )
        SAVE_PATH = os.path.join(Config.OUT_DIR, os.path.relpath(file, Config.TEST_DIR))
        os.makedirs(os.path.dirname(SAVE_PATH), exist_ok=True)
        with open(SAVE_PATH + ".csv.log", "w") as f:
            return run(
                [EXEC, file]
                + Config.INCLUDE.split()
                + [
                    "--lcom:debug",
                    Config.LOG_LEVEL,
                    "--lcom:dot-behavior",
                    Config.DOT_BEHAVIOR,
                    "--lcom:class-type",
                    Config.CLASS_TYPE,
                    "--lcom:csv-output",
                    SAVE_PATH + ".csv",
                ]
                + Config.ARGS.split(),
                f,
                os.path.join(os.path.dirname(SAVE_PATH), "workingDir-lcom"),
            )

    src_files = list_files(Config.DIR, ["adb", "cpp"], use_exclusions=True)
    wait_list = []
    for file in src_files:
        wait_list.append(lcom(file))
    for task in wait_list:
        task.wait()


def combine_csv():
    wait("gen_lcom", optional=True)
    print("Combining LCOM metrics into a CSV")
    csv_output = b""
    # Read in the header.
    with open(os.path.join(Config.PROJECT, "header.csv"), "rb") as header:
        csv_output += header.read()
    # Read in the remaining files.
    csv_files = list_files(Config.OUT_DIR, ["csv"], use_exclusions=False)
    for file in csv_files:
        with open(file, "rb") as file_contents:
            csv_output += file_contents.read()
    # Write the output to file.
    wait("wipe_output", optional=True)
    os.makedirs(Config.OUT_DIR, exist_ok=True)
    with open(os.path.join(Config.OUT_DIR, "lcom.csv"), "wb") as lcom_file:
        lcom_file.write(csv_output)
    return


def make_lcom_dot_graphs():
    EXEC = os.path.join(Config.BUILD_DIR, "lcom-dot.out")
    wait("build")
    wait("wipe_output", optional=True)
    print("Generating LCOM DOT graphs")

    def make_lcom_dot_graph(file):
        print(
            "Generating LCOM DOT graph for "
            + file.encode("utf-8", "backslashreplace").decode()
        )
        SAVE_PATH = os.path.join(Config.OUT_DIR, os.path.relpath(file, Config.TEST_DIR))
        os.makedirs(os.path.dirname(SAVE_PATH), exist_ok=True)
        with open(SAVE_PATH + ".lcom.dot.log", "w") as f:
            return run(
                [EXEC, file]
                + Config.INCLUDE.split()
                + [
                    "--lcom:debug",
                    Config.LOG_LEVEL,
                    "--lcom:dot-behavior",
                    Config.DOT_BEHAVIOR,
                    "--lcom:class-type",
                    Config.CLASS_TYPE,
                    "--lcom:dot-output",
                    os.path.dirname(SAVE_PATH),
                ]
                + Config.ARGS.split(),
                f,
                os.path.join(os.path.dirname(SAVE_PATH), "workingDir-lcomdot"),
            )

    src_files = list_files(Config.DIR, ["adb", "cpp"], use_exclusions=True)
    wait_list = []
    for file in src_files:
        wait_list.append(make_lcom_dot_graph(file))
    for task in wait_list:
        task.wait()


def run_gtests():
    wait("build")
    wait("wipe_output", optional=True)
    print("Running unit tests")
    tests = run(
        [
            os.path.join(os.environ["GTEST_REPO"], "gtest-parallel"),
            os.path.join(Config.BUILD_DIR, "lcom-unittest"),
        ],
        cwd=os.path.join(Config.OUT_DIR, "workingDir-gtest"),
    )
    tests.wait()


def cleanup():
    print("Cleaning up")
    subfolders = find_subfolders(Config.OUT_DIR, "workingDir")
    for folder in subfolders:
        shutil.rmtree(folder)
    subfolders = find_subfolders(Config.OUT_DIR, "gnatOutput")
    for folder in subfolders:
        shutil.rmtree(folder)
    trash_files = list_files(
        Config.DIR, ["dot", "lcom", "csv", "li", "o", "ti", "out"], use_exclusions=False
    )
    for file in trash_files:
        os.remove(file)


def dict_to_str_list(d):
    option = []
    for key in d.keys():
        option.append(str(key))
    return option


class TaskAction(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        if not values:  # if no arguments are passed
            values = list(deps.keys())
        elif not all(
            task in deps.keys() for task in values
        ):  # if invalid argument is passed
            raise argparse.ArgumentError(
                self,
                "invalid choice: {} (choose from {})".format(
                    ", ".join(values), ", ".join(deps.keys())
                ),
            )
        setattr(namespace, self.dest, values)


def main():
    # All main tasks should be defined here for use as needed.
    global deps
    global tasks_to_execute
    functions = [
        build,
        wipe_output,
        make_dot_graphs,
        gen_lcom,
        make_lcom_dot_graphs,
        combine_csv,
        run_gtests,
    ]
    for func in functions:
        deps[func.__name__] = DeferredTask(func)

    parser = argparse.ArgumentParser(description="Run LCOM tests.")
    parser.add_argument(
        "task_names",
        nargs="*",
        help="Names of tasks to execute. Specify one or more. If none are specified, all tasks will run. \nChoices: ["
        + ", ".join(deps.keys())
        + "]",
        action=TaskAction,
    )
    parser.add_argument(
        "--project",
        type=str,
        help=f'Root directory of project. (default: "{Config.PROJECT}")',
    )
    parser.add_argument(
        "--dot_gen",
        type=str,
        help=f'Full path to dotGenerator executable. Needed by make_dot_graphs task. (default: "{Config.dotGenerator}")',
    )
    parser.add_argument(
        "--include",
        type=str,
        help=f'A custom include string. (default: "{Config.INCLUDE}")',
    )
    parser.add_argument(
        "--job_limit",
        type=int,
        help=f'The maximum number of tasks to execute at once. (default: "{Config.JOB_LIMIT}")',
    )
    parser.add_argument(
        "--out_dir",
        type=str,
        help=f'Directory to output tests to. (default: "{Config.OUT_DIR}")',
    )
    parser.add_argument(
        "--log_level",
        type=str,
        help=f'Logging level for LCOM tool. (default: "{Config.LOG_LEVEL}")',
        choices=["trace", "debug", "info", "notice", "warning", "error", "fatal"],
    )
    parser.add_argument(
        "--class_type",
        type=str,
        help=f'The local program unit to evaluate LCOM on (e.g., class, function, etc.) (default: "{Config.CLASS_TYPE}").',
        choices=["Default", "Package", "Function", "Class", "ProtectedObject", "All"],
    )
    parser.add_argument(
        "--dot_mode",
        type=str,
        help=f'The desired DOT behavior. (default: "{Config.DOT_BEHAVIOR}")',
        choices=["LeftOnly", "Full"],
    )
    parser.add_argument(
        "--dir",
        type=str,
        help=f'A directory path relative to the testcases folder. Also supports file paths. (e.g., "simple-cases/c1.adb") (default: "{Config.DIR}")',
    )
    parser.add_argument(
        "--args",
        type=str,
        help=f'Additional arguments passed directly to lcom.out and lcom_dot.out. Argparse assumes values with - or -- to be mistakes, so you must use the pseudo-argument \'--\' to handle this. (e.g., "-- --lcom:anonymous") (default: "{Config.ARGS}")',
    )
    # TODO: May want to keep the joined portions of these somewhere else, as
    # they are now used here and in the Config class initializer.
    args = parser.parse_args()
    if args.task_names:
        tasks_to_execute = args.task_names
    else:
        tasks_to_execute = dict_to_str_list(deps)
    if args.project is not None:
        Config.PROJECT = args.project
        # Update dependant paths.
        Config.INCLUDE = "-I" + os.path.join(Config.PROJECT, "testcases/acats/support")
        Config.TEST_DIR = os.path.join(Config.PROJECT, "testcases")
        Config.OUT_DIR = os.path.join(Config.PROJECT, "output")
        Config.BUILD_DIR = os.path.join(Config.PROJECT, "build")

        Config.ERROR_LOG = os.path.join(Config.OUT_DIR, "failures.log")
        Config.DIR = os.path.join(Config.TEST_DIR, "simple-cases")
    if args.dot_gen is not None:
        Config.dotGenerator = args.dot_gen
    if args.include is not None:
        Config.INCLUDE = args.include
    if args.job_limit is not None:
        Config.JOB_LIMIT = args.job_limit
    if args.out_dir is not None:
        Config.OUT_DIR = args.out_dir
        Config.ERROR_LOG = os.path.join(Config.OUT_DIR, "failures.log")
    if args.log_level is not None:
        Config.LOG_LEVEL = args.log_level
    if args.class_type is not None:
        Config.CLASS_TYPE = args.class_type
    if args.dot_mode is not None:
        Config.DOT_BEHAVIOR = args.dot_mode
    if args.dir is not None:
        Config.DIR = os.path.join(Config.TEST_DIR, args.dir)
    if args.args is not None:
        Config.ARGS = args.args

    for task in tasks_to_execute:
        execute(task)
    for task in tasks_to_execute:
        wait(task)

    cleanup()
    return


if __name__ == "__main__":
    main()
