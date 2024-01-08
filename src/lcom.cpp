#include "rose.h"
// Always include ROSE first.

#include <Rose/CommandLine.h>
#include <Sawyer/CommandLine.h>

#include <boost/filesystem.hpp>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "aixlog.hpp"
#include "define.hpp"
#include "lcom.hpp"
#include "traverse.hpp"

// using Class = SgAdaPackageSpec*;
using Method = Traverse::MType;
using Attribute = Traverse::AType;

static const char* purpose = "lcom tool";
static const char* description =
    "Generates LCOM measurements for Ada packages in a single project and "
    "saves the output to CSV.";

struct Settings {
  boost::filesystem::path csvPath;
  ClassType classType = ClassType::Default;
};

std::tuple<std::vector<std::string>, Settings> parseArgs(
    std::vector<std::string> args, Settings settings, int argc, char* argv[]) {
  namespace scl = Sawyer::CommandLine;
  scl::Parser p =
      Rose::CommandLine::createEmptyParserStage(purpose, description);
  p.with(Rose::CommandLine::genericSwitches());

  // Create a group of switches specific to this tool
  scl::SwitchGroup lcomArgs("Lack of Cohesion of Methods (LCOM) switches");
  lcomArgs.name("lcom");
  lcomArgs.insert(scl::Switch("debug")
                      .argument("[trace|debug|info|notice|warning|error|fatal]",
                                scl::enumParser<AixLog::Severity>(debug)
                                    ->with("trace", AixLog::Severity::trace)
                                    ->with("debug", AixLog::Severity::debug)
                                    ->with("info", AixLog::Severity::info)
                                    ->with("notice", AixLog::Severity::notice)
                                    ->with("warning", AixLog::Severity::warning)
                                    ->with("error", AixLog::Severity::error)
                                    ->with("fatal", AixLog::Severity::fatal))
                      .doc("Specifies a logging severity level."));
  lcomArgs.insert(scl::Switch("anonymous")
                      .intrinsicValue("true", scl::booleanParser(anonymous))
                      .doc("Disable component name resolution."));
  lcomArgs.insert(
      scl::Switch("csv-output", 'o')
          .argument("filename", scl::anyParser(settings.csvPath))
          .doc("Path and optional name to store csv output. By default, the "
               "file is stored in the same directory as the analyzed file, "
               "under the name \"<sourceName>.adb.csv\"."));
  lcomArgs.insert(
      scl::Switch("dot-behavior")
          .argument("[LeftOnly|Full]",
                    scl::enumParser<DotBehavior>(dotBehavior)
                        ->with("LeftOnly", DotBehavior::LeftOnly)
                        ->with("Full", DotBehavior::Full))
          .doc("Specifies the behavior of record field accesses.\n"
               "\tLeftOnly: Any access to the left or right part of a dot "
               "operator is viewed as an access to only the left part.\n"
               "\tFull: Any access to the right part of the dot operator is "
               "viewed as an access to the right part, but it is disambiguated "
               "from other instances of the record using the left part. Any "
               "access to the left part is viewed as an overlapping access to "
               "any right parts contained within.\n"));
  lcomArgs.insert(
      scl::Switch("class-type")
          .argument("[Default|Package|Function|Class|ProtectedObject|All]",
                    scl::enumParser<ClassType>(settings.classType)
                        ->with("Package", ClassType::Package)
                        ->with("Function", ClassType::Function)
                        ->with("Class", ClassType::Class)
                        ->with("ProtectedObject", ClassType::ProtectedObject)
                        ->with("Default", ClassType::Default)
                        ->with("All", ClassType::All))
          .doc(std::string(
                   "The program unit to use as the LCOM class. \"All\" will "
                   "run analysis on all predefined class types. Defaults to ") +
               typeid(Class).name() + "."));
  scl::ParserResult cmdline = p.with(lcomArgs).parse(args).apply();

  // Initialize the logger here.
  // AixLog::Log::init<AixLog::SinkCout>(debug);
  AixLog::Log::init<AixLog::SinkCallback>(
      debug, [](const AixLog::Metadata& metadata, const std::string& message) {
        std::cout << "[" << AixLog::to_string(metadata.severity) << "] "
                  << message;
        if (metadata.severity >= AixLog::Severity::warning) {
          std::cout << " [" << metadata.function.file << ":"
                    << metadata.function.line << "]";
        }
        if (metadata.severity >= AixLog::Severity::fatal) {
          assert(false);
        }
        std::cout << std::endl;
      });

  return std::make_tuple(cmdline.unparsedArgs(), settings);
}

template <typename C>
std::string ProcessLCOM(SgProject*& project) {
  std::stringstream ss;
  const std::vector<LCOM::Class<C, Method, Attribute>> LCOMInput =
      Traverse::GetClassData<C>(project);
  // Compute LCOM metrics for each class.
  for (const auto& LCOMClass : LCOMInput) {
    std::string className = "null";
    boost::filesystem::path sourceFile = Traverse::sourceFile;
    if (is<C>(LCOMClass.GetId())) {
      Traverse::Class<C>& classObj =
          Traverse::IA<C>::classData.at(LCOMClass.GetId());
      className = NPrint::p(classObj.GetId());
      sourceFile = classObj.sourceFile;
    }
    std::cout << "Class: " << className << std::endl;
    LCOM::LCOM1Data data1;
    LCOM::LCOM5Data data5;
    // Get the LCOM measurements.
    const std::size_t lcom1 = LCOM::LCOM1(LCOMClass, &data1);
    std::cout << "LCOM1: " << lcom1 << std::endl;
    const std::size_t lcom2 = LCOM::LCOM2(LCOMClass);
    std::cout << "LCOM2: " << lcom2 << std::endl;
    const std::size_t lcom3 = LCOM::LCOM3(LCOMClass);
    std::cout << "LCOM3: " << lcom3 << std::endl;
    const std::size_t lcom4 = LCOM::LCOM4(LCOMClass);
    std::cout << "LCOM4: " << lcom4 << std::endl;
    const double lcom5 = LCOM::LCOM5(LCOMClass, &data5);
    std::cout << "LCOM5: " << lcom5 << std::endl;
    // Normalized.
    std::cout << "LCOM1Norm: " << (double)lcom1 / (double)data1.totalPairs
              << std::endl;
    std::cout << "LCOM2Norm: " << (double)lcom2 / (double)data1.totalPairs
              << std::endl;
    std::cout << "LCOM3Norm: " << (double)lcom3 / (double)data5.k << std::endl;
    // NOTE: Normalized LCOM4 is basically YALCOM without special 0 and 1 cases.
    // https://www.tusharma.in/yalcom-yet-another-lcom-metric.html
    std::cout << "LCOM4Norm: " << (double)lcom4 / (double)data5.k << std::endl;

    // Generate a line of CSV.
    ss << sourceFile << ",\"" << className << "\",\"" << typeid(C).name()
       << "\",\"" << dotBehavior << "\",";
    ss << lcom1 << "," << lcom2 << "," << lcom3 << "," << lcom4 << "," << lcom5
       << ",";
    ss << data1.sharedPairs << "," << data1.unsharedPairs << ","
       << data1.totalPairs << ",";
    ss << data5.a << "," << data5.l << "," << data5.k << ",";
    ss << (double)lcom1 / (double)data1.totalPairs << ",";
    ss << (double)lcom2 / (double)data1.totalPairs << ",";
    ss << (double)lcom3 / (double)data5.k << ",";
    ss << (double)lcom4 / (double)data5.k << "," << std::endl;
  }
  return ss.str();
}

int main(int argc, char* argv[]) {
  ROSE_INITIALIZE;
  std::vector<std::string> cmdLineArgs{argv + 1, argv + argc};
  Settings settings;
  std::tie(cmdLineArgs, settings) =
      parseArgs(std::move(cmdLineArgs), settings, argc, argv);
  if (!anonymous) {
    LOG(DEBUG) << "Remaining args:";
    for (auto i = cmdLineArgs.begin(); i != cmdLineArgs.end(); ++i) {
      LOG(DEBUG) << *i << ' ';
    }
    LOG(DEBUG) << std::endl;
  }
  // GetProject expects to see the name of the executable as the first argument.
  // Restore it.
  cmdLineArgs.insert(cmdLineArgs.begin(), argv[0]);

  // Print out the full command, to make it easy to reproduce the test.
  std::vector<std::string> printArgs{argv, argv + argc};
  std::stringstream cmdStream;
  for (const auto& arg : printArgs) {
    cmdStream << arg << " ";
  }
  LOG(INFO) << "Running command: " << cmdStream.str() << std::endl;

  SgProject* project = Traverse::GetProject(cmdLineArgs);
  // CSV line output.
  std::stringstream ss;
  LOG(DEBUG) << "Running analysis for class type: " << settings.classType
             << std::endl;
  switch (settings.classType) {
    case ClassType::Package:
      ss << ProcessLCOM<SgAdaPackageSpec*>(project);
      break;
    case ClassType::Function:
      ss << ProcessLCOM<SgFunctionDeclaration*>(project);
      break;
    case ClassType::Class:
      ss << ProcessLCOM<SgClassDeclaration*>(project);
      break;
    case ClassType::ProtectedObject:
      ss << ProcessLCOM<SgAdaProtectedSpec*>(project);
      break;
    case ClassType::Default:
      LOG(INFO) << "No/invalid class type specified. Running analysis on "
                   "default type, "
                << typeid(Class).name() << "." << std::endl;
      ss << ProcessLCOM<Class>(project);
      break;
    case ClassType::All:
      ss << ProcessLCOM<SgAdaPackageSpec*>(project);
      ss << ProcessLCOM<SgFunctionDeclaration*>(project);
      ss << ProcessLCOM<SgClassDeclaration*>(project);
      ss << ProcessLCOM<SgAdaProtectedSpec*>(project);
  }

  // Output the string to file.
  const boost::filesystem::path defaultPath =
      Traverse::sourceFile.string() + ".csv";
  if (settings.csvPath.empty()) {
    if (anonymous) {
      LOG(ERROR)
          << "The path was anonymized and thus cannot be used as an output "
             "path. Please manually set a CSV path using the "
             "--lcom::csv-output argument if you want to save the output."
          << std::endl;
      return -1;
    }
    settings.csvPath = defaultPath;
  } else {
    if (settings.csvPath.filename().empty()) {
      settings.csvPath =
          settings.csvPath.parent_path() / defaultPath.filename();
    }
  }
  std::ofstream of(settings.csvPath.string());
  if (of.is_open()) {
    LOG(DEBUG) << "Outputting " << ss.str() << " to " << settings.csvPath
               << std::endl;
    of << ss.str();
  }
  of.close();

  return 0;
}
