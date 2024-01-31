# LCOM Metric Analyzer

## Description

LCOM Metric Analyzer is a tool that evaluates the quality of object-oriented code by identifying classes that lack cohesion.
These classes can be broken down into smaller, more manageable units for improved clarity and maintenance.
Cohesion is measured using the various LCOM (Lack of Cohesion of Methods) metrics, and source code is processed using the ROSE compiler infrastructure.

## Example

Given the following source code for the class in [c4.adb](testcases/simple-cases/c4.adb):

```ada
package body c4 is
    procedure m1 is
    begin
        a1 := 1;
        a2 := 2;
    end m1;
    procedure m2 is
    begin
        a1 := 1;
        m3;
    end m2;
    procedure m3 is
    begin
        a3 := 3;
    end m3;
    procedure m4 is
    begin
        a4 := 4;
        m3;
    end m4;
end c4;
```

Our tool converts it into an LCOM graph, where each method is a box, each attribute is an ellipse, and arrows indicate a method access.
Note how methods can call other methods, with arrows between boxes indicating the calls.

![An LCOM DOT graph for c4.adb](resources/c4.svg)

This graph is analyzed by the tool using the [definitions of LCOM](#lcom-definitions-in-plain-english) to compute the lack of cohesion of methods for the class.

| LCOM1 | LCOM2 | LCOM3 | LCOM4 | LCOM5 |
|-------|-------|-------|-------|-------|
| 5     | 4     | 3     | 1     | 11/12 |

## Dependencies

- GNAT compiler: https://www.gnu.org/software/gnat/
- BOOST libraries: https://www.boost.org/
- ROSE compiler tool: https://github.com/rose-compiler/rose

Build and install [GNAT](https://www.gnu.org/software/gnat/) and [BOOST](https://www.boost.org/), then set the locations of each install location in the running environment so that we can build [ROSE](https://github.com/rose-compiler/rose) and the LCOM Metric Analyzer:

### Example environment
```bash
export GNAT_HOME=/PATH/TO/GNAT2019
export PATH="$GNAT_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$GNAT_HOME/lib64:$GNAT_HOME/lib:$LD_LIBRARY_PATH"

export BOOST_HOME=/PATH/TO/BOOST
export LD_LIBRARY_PATH=$BOOST_HOME/lib:$LD_LIBRARY_PATH
export BOOST_ROOT=$BOOST_HOME
export BOOST_LIB=$BOOST_ROOT/libexport

export ROSE_HOME=/PATH/TO/ROSE/INSTALL_TREE
export ROSE_ROOT=$ROSE_HOME

export ASIS_ADAPTER=/PATH/TO/ROSE/BUILD/TREE/src/frontend/Experimental_Ada_ROSE_Connection/parser/asis_adapter
```

### Building ROSE

Build ROSE with Ada language support with the following commands.

```bash
git clone https://github.com/rose-compiler/rose.git &&
cd rose &&
./build &&
mkdir -p build_tree &&
cd build_tree &&
../configure --prefix=$ROSE_HOME --enable-languages=c,c++ --enable-experimental_ada_frontend --without-swi-prolog --without-cuda --without-java --without-python --with-boost=$BOOST_HOME --with-boost-libdir=$BOOST_HOME/lib --verbose --with-DEBUG=-ggdb --with-alloc-memset=2 --with-OPTIMIZE="-O0 -march=native -p" --with-WARNINGS="-Wall -Wextra -Wno-misleading-indentation -Wno-unused-parameter" CXX=$GNAT_HOME/bin/g++ CC=$GNAT_HOME/bin/gcc &&
make clean -j$(nproc) &&
make -j$(nproc) &&
make install -j$(nproc) &&
make check -j$(nproc) &&
cd exampleTranslators &&
make -j$(nproc)
```

### Optional tests

This tool has been tested with additional sources not included in this repo.
You can prepare them for use with this tool using the following commands.

#### ACATS

ROSE is designed to support Ada 95, so we use the associated ACATS version, 2.6.
`acats.sh` can be used to download and process the source into `testcases/acats`.

#### Open-source code

A small collection of open source code that uses the Ada 95 language.

| Project                                                                               | Ada lines of code |
|---------------------------------------------------------------------------------------|------------------:|
| [Ada Exploiting](https://github.com/bkungl/AdaExploiting)                             | 1,675             |
| [Ada Structured Library](https://sourceforge.net/projects/adasl/)                     | 48,258            |
| [ALIRE: Ada LIbrary REpository](https://github.com/alire-project/alire)               | 50,999            |
| [Ada 95 Booch Components](https://sourceforge.net/projects/booch95/)                  | 34,073            |
| [Simple components for Ada](https://sourceforge.net/projects/simplecomponentsforada/) | 463,660           |
| [Fuzzy sets for Ada](https://sourceforge.net/projects/fuzzysetsforada/)               | 695,430           |
| [GNAT Studio](https://github.com/AdaCore/gnatstudio)                                  | 845,908           |
| [Libadalang-tools](https://github.com/AdaCore/libadalang-tools)                       | 140,481           |
| [LinXtris](https://sourceforge.net/projects/linxtris/)                                | 5,341             |
| [PHCpack](https://github.com/janverschelde/PHCpack)                                   | 2,492,729         |
| [PNG_IO](https://sourceforge.net/projects/png-io/)                                    | 4,214             |
| [SHA-1](https://github.com/UlrikHjort/SHA-1)                                          | 498               |
| [Ada KALINDA OS](https://sourceforge.net/projects/sx-ada-kalinda/)                    | 20,383            |

To download the source into `testcases/osc`, run `osc.sh`
This will download all of the projects in parallel.

#### simple-cpp-programs

Some basic C++ programs.
These were used to verify functionality of the LCOM tool on C++ code.

```bash
git clone https://github.com/amngupta/simple-cpp-programs.git testcases/simple-cpp-programs
```

## Building and testing the tool

The recommended process uses [cmake](CMakeLists.txt). A [Makefile](Makefile) is also provided.

```bash
rm -r build/ # Remove the build directory to ensure a fresh build (rarely needed)
cmake -S . -B build # Create the build directory with autogenerated makefiles
cmake --build build --parallel $(nproc) # Build all LCOM tools in parallel
pushd build && ctest; popd # Run GTests sequentially
/PATH/TO/TESTDIR/gtest-parallel/gtest-parallel build/lcom-unittest # Alternatively run GTests in parallel
```

### Full build command

As an alternative to a proper build system, you can also build directly with the following command.

```bash
g++ -o lcom.out src/lcom.cpp -Iinclude -I${ROSE_ROOT}/include/rose -I${BOOST_ROOT}/include -lrose -lboost_date_time -lboost_thread -lboost_filesystem -lboost_program_options -lboost_regex -lboost_system -lboost_serialization -lboost_wave -lboost_iostreams -lboost_chrono -ldl -lm -lquadmath -lasis_adapter -lstdc++fs -pthread -L${ROSE_ROOT}/lib -L${BOOST_ROOT}/lib -L$ASIS_ADAPTER/lib -Wl,-rpath ${BOOST_ROOT}/lib -Wl,-rpath $ASIS_ADAPTER/lib -Wl,-rpath=${ROSE_ROOT}/lib
```

## Usage

### Quick start

Run `bash allTest.sh` to run tests.

### Custom run

1. Copy any source code into `testcases/<your directory here>` for evaluation.
1. Many options can be specified. View them with `python3 scripts/allTest.py -h`
1. Run `python3 scripts/allTest.py <task>`, specifying which tasks you want to run. The following tasks are available:
- build
- wipe_output
- make_dot_graphs
- gen_lcom
- make_lcom_dot_graphs
- combine_csv
- run_gtests

Multiple tasks, each separated by a space, can be selected to run in one test.
If no task is specified, all of them will run.

## High-level code overview

### [AST Traversal](include/traverse.hpp)

Abstract syntax tree traversal is the most complex step.
At a high level, it works using ROSE's AstTopDownProcessing visitor traversal pattern, a recursive pattern that accesses each node from the top down.
This ensures every node is evaluated.
An inherited attribute is copied down to child nodes, but all inherited attributes are static in our approach.
Whenever one of our target node types is seen (e.g., Class=`SgAdaPackageSpec*`, Method=`SgFunctionDeclaration*`, Attribute=`SgInitializedName*`), we process its relationship with the other nodes.

#### Class

The class is the simplest.
When a class node is seen, it is added to the list of classes.

#### Method

When a method is seen, it is added to the list of methods and associated to its parent class.
The parent class is identified by traversing up to parent scopes until a matching class type is seen.

#### Attribute

When an attribute is seen, it is added to the list of attributes and associated to its parent method.
The parent method is identified by traversing up to parent scopes until a matching method type is seen.

#### Called Method

When a method is called, it is associated to the method that called it by traversing up to parent scopes until a matching method type is seen.

#### Ada renamings

Attributes and methods can be renamed in Ada, complicating the process of identifying methods that share attributes.
When a renaming, pointer, etc. is seen, it is stored in a renaming map, which can be used to look up the root attribute or method associated with the call.

#### Ada records

Record are used to contain fields, allowing multiple attributes to be tied to a single object.
If a record is accessed, it needs to be seen as overlapping with any access to any underlying fields as well.
Since record fields can themselves be records, it is possible that a field will be created and later accessed several records down.
To accommodate this, we use a tree data structure, where each node is an attribute and children are fields.
Every method access to a specific attribute is associated with the corresponding node.
To find overlapping method accesses, traverse up from each leaf node to the root, connecting all methods found along each leaf-to-root traversal.

### [LCOM](include/lcom.hpp)

Once the ROSE AST has been traversed and the relationships between classes, methods, and attributes is captured, LCOM1-5 are calculated using the standard approaches [outlined here](https://www.ece.rutgers.edu/~marsic/books/SE/instructor/slides/lec-16%20Metrics-Cohesion.ppt).
Additional, normalized metrics are computing by taking the LCOM metric divided by lowest possible cohesion for a class with the given number of methods, where 1 is least cohesive and 1/#methods is the most cohesive.

#### LCOM definitions, in plain English

- [LCOM1](https://dl.acm.org/doi/10.1109/32.295895): The number of pairs of methods that do not share attributes.
- [LCOM2](https://dl.acm.org/doi/10.1109/32.295895): The number of pairs of methods that do not share attributes minus the number of pairs of methods that do share attributes.
- [LCOM3](https://www.sciencedirect.com/science/article/pii/016412129390077B): The number of connected components in the graph that represents each method as a node and the sharing of at least one attribute as an edge.
- [LCOM4](https://ieeexplore.ieee.org/document/491650): The number of connected components in the graph that represents each method as a node and the sharing of at least one attribute as an edge. Edges between methods also form when one method calls another within the same class.
- LCOM5: The sum of non-module attributes accessed by a class, defined by the formula `(a-k*l)/(l-k*l)`, where `a` is the number of attribute accesses, `l` is the number of attributes, and `k` is the number of methods.

### Ada - What is a class?

Ada has no specific class construct in the language, so identifying an object that could be used as a class is non-obvious.
Anything with a distinct [scope](https://perso.telecom-paristech.fr/pautet/Ada95/chap09.htm) in Ada could reasonably be used as a class.
We elected to use the same eligible local program units chosen by [GNATmetric](https://www.adacore.com/static-analysis/gnatmetric) as our class types: packages, functions/procedures/subprograms, and protected objects.
GNATmetric also evaluates tasks.
In this situation, entries are methods.
However, these is no clear way for an entry to reference an attribute, so LCOM is not meaningful here.

### Correctness, assumptions, and limitations

To determine if our tool produces the correct results, we must first report our assumptions.

- Attributes that are never accessed do not count toward the attribute count used to calculate LCOM5.
- All methods within a class are considered, even if they have no accessed attributes.
- If an attribute is accessed multiple times within a method, it is seen as only a single access. This can affect LCOM5.
- If an attribute is never accessed, its existence will not be reported by the tool. This can affect LCOM5.
- An array is seen as a single attribute for the purposes of LCOM.
- When DotBehavior is set to LeftOnly, access to a record field counts as an access to the object as a whole. In this situation, we do not track individual fields as unique attributes.
- When DotBehavior is set to Full, access to a record field counts as a single access to the specific field. If a field access overlaps with another (potentially parent) field or record access, it is counted as a shared access.
- Attribute accesses and method calls made outside of a method are ignored.
- Methods outside of a class are ignored.
- Constructors and initializers are not considered valid methods for the purposes of LCOM, much like [related work](https://github.com/potfur/lcom).
- Access to data outside of the currently evaluated file are often inaccessible. What is analyzed by the LCOM tool is limited to the contents of the AST generated by ROSE. For instance, analyzing [point_complex.adb](testcases/other-tests/point_complex.adb) can find the declarations in [gcomplex.ads](testcases/other-tests/gcomplex.ads) but misses the attribute accesses made in the method definitions found in [gcomplex.adb](testcases/other-tests/gcomplex.adb) because they are not in the AST.

## Finding and reporting issues
This tool has been tested for functionality with Ada and, to a lesser extent, C++.
In principle, it should be compatible with all languages that work with ROSE, although coverage of language features have only been thoroughly investigated for Ada.

Issues with the tool are likely to be reported in the form of warning/error/fatal logs, unexpected program termination, or assertion failure.
These can be tracked down in logs using the following RegEx:
```regex
\[(Fatal|Error|Warn)\]|(terminate called after)|(Assertion )
```
Issue reports for this tool should include the relevant logs (at trace level) and source files.

## Remaining work

### Support for additional programming languages
While other languages are handled by ROSE, this tool has only been tested extensively on Ada code and found to work with C++ code.
However, languages are complex and this tool may require significant additional work to handle complex language features in a reasonable way.
Work could be done to improve support for other languages.

### Investigate existing designs
LCOM is a simple metric on its own, but real-world programming languages add complexity when considering a variety of language features.
We should more deeply explore how existing designs handle those edge cases when calculating LCOM.

### Experiment with nested access behavior
Currently, all attribute and called method accesses are associated only with their immediate scope parent class.
It would be interesting to see how LCOM changes if we associated that access with all parent classes, to handle nested classes.
It would also be helpful to explore how [other LCOM tools](#related-works-and-resources) accommodate nesting.
Once this is implemented, an associated test case should be made for [child.adb](testcases/other-tests/child.adb) to ensure it works properly.

### Associate attributes in a called method back to the caller
When a method calls another method, it indirectly accesses all of the attributes within that called method.
We should see what happens to LCOM if we recursively identify these attributes and associate them back to the calling method.

### Count number of fields within a record
LCOM5 currently counts the appearance of an attribute node as a single attribute access.
However, when that attribute is a record, it has multiple fields associated with it, each of which can be seen as a separate attribute access for the purposes of LCOM5.
It may be worth tracking the number of underlying fields associated with each record access to report a more accurate LCOM5 metric.

### Ada - Handle a tagged type's methods
Free-standing methods take the tagged type (a special kind of record) as the first argument, effectively making them methods associated with the tagged type "class", even though the tagged type is not part of the method's parent scope.
The AST should already provide enough information to identify these methods and properly associate them with the owning tagged type.
Once completed, a test case should be created for [p.adb](testcases/other-tests/p.adb)  in [lcom-unittest.cpp](src/lcom-unittest.cpp)to ensure tagged types are handled correctly.

### Ada - Integrate with [Ada Analysis Toolkit](https://rosecompiler2.llnl.gov/gitlab/rose-tools/ada-analysis-toolkit)
The Ada analysis toolkit is a useful visualization tool to evaluate a codebase.
LCOM could be integrated into this tool to color-code methods by LCOM and display their relationships in the code.

### Release this software to the public
LLNL has a [process](https://dev.llnl.gov/opensource/releasing/) to release source code under a permissive open source license.
We should work to finish the publication process.
It has already been started and can be continued on the [eSoftware dashboard](https://esw.llnl.gov/).

### Miscellany
- Fix NPrint::p() in [node-print.hpp](include/node-print.hpp) to use a hierarchy of supported print functions.
- Fix Cache in [lcom.hpp](include/lcom.hpp).

## Related works and resources

- [Class Cohesion Metrics for Software Engineering: A Critical Review](https://www.math.md/files/csjm/v25-n1/v25-n1-(pp44-74).pdf): Overview of cohesion state of the art
- [Refactoring Effect on Cohesion Metrics](https://ieeexplore.ieee.org/abstract/document/5328998): Evaluation of how effective cohesion methods are as refactoring aids
- [LCOM Lecture Slides](https://www.ece.rutgers.edu/~marsic/books/SE/instructor/slides/lec-16%20Metrics-Cohesion.ppt): [archive.org backup](http://web.archive.org/web/20220307222105/https://www.ece.rutgers.edu/~marsic/books/SE/instructor/slides/lec-16%20Metrics-Cohesion.ppt)
- [Cohesion metrics](https://www.aivosto.com/project/help/pm-oo-cohesion.html): Description of LCOM
- [YALCOM](https://www.tusharma.in/yalcom-yet-another-lcom-metric.html): Yet Another LCOM Metric
- [LCOM](https://github.com/tushartushar/LCOM): Java LCOM implementation
- [jpeek](https://github.com/cqfn/jpeek): Alternative Java LCOM implementation
- [lcom](https://github.com/potfur/lcom): Python-based LCOM implementation
- [LCOM4go](https://github.com/yahoojapan/lcom4go): Golang-based LCOM4 implementation

# LLNL Software Release

LLNL-CODE-858757

