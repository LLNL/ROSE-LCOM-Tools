// Generate DOT graphs to visualize LCOM relationships.
#include "rose.h"
// Always include ROSE first.

#include <Rose/CommandLine.h>
#include <Sawyer/CommandLine.h>

#include <boost/filesystem.hpp>
#include <set>
#include <sstream>
#include <string>

#include "aixlog.hpp"
#include "is-type-rose.hpp"
#include "traverse.hpp"

// using Class = SgAdaPackageSpec*;
using Method = Traverse::MType;
using Attribute = Traverse::AType;

static const char* purpose = "LCOM tool DOT generator";
static const char* description =
    "Generates LCOM DOT graphs for Ada packages in a single project and "
    "saves the output to DOT.";

struct Settings {
  boost::filesystem::path dotPath;
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
      scl::Switch("dot-output", 'o')
          .argument("filename", scl::anyParser(settings.dotPath))
          .doc("Path to store dot output. By default, the file is stored in "
               "the same directory as the analyzed file."));
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

std::string PName(const SgNode* n) {
  std::stringstream ss;
  ss << "p" << size_t(n);
  return ss.str();
}

template <typename C>
void GenerateLCOMGraphs(SgProject*& project, const Settings& settings) {
  const std::vector<LCOM::Class<C, Method, Attribute>> LCOMInput =
      Traverse::GetClassData<C>(project);
  for (const auto& classInst : LCOMInput) {
    std::string className = "null";
    if (is<C>(classInst.GetId())) {
      Traverse::Class<C>& classInstObj =
          Traverse::IA<C>::classData.at(classInst.GetId());
      std::stringstream ss;
      ss << classInstObj;
      className = ss.str();
    } else {
      LOG(WARNING) << "No class name found for " << classInst << std::endl;
    }
    const boost::filesystem::path sourceFile =
        Traverse::IA<C>::classData.at(classInst.GetId()).sourceFile;
    boost::filesystem::path outfile;

    if (settings.dotPath.empty()) {
      outfile = sourceFile;
    } else {
      outfile = settings.dotPath / sourceFile.filename();
    }
    outfile = outfile.parent_path() /
              boost::filesystem::path(outfile.filename().string() + "_" +
                                      className + ".lcom.dot");
    LOG(NOTICE) << "Saving to " << outfile << std::endl;
    std::fstream out(outfile.string(), std::ios::out);
    LCOMToDOT(out, classInst);
  }
}

void Header(std::ostream& os) {
  os << "digraph {" << std::endl;
  os << "  compound=true;" << std::endl;
  os << "  rankdir=\"BT\"" << std::endl;
  os << "  style=\"rounded\";" << std::endl;
}

void Footer(std::ostream& os) { os << "}" << std::endl; }

void Edge(std::ostream& os, const std::string& src, const std::string& tgt,
          const std::string& lbl, const std::string& attr) {
  os << "  " << src << " -> " << tgt << "[ taillabel = \"" << lbl << "\" "
     << attr << "];" << std::endl;
}

void Node(std::ostream& os, const std::string& n, std::string lbl,
          std::string attr) {
  os << "  " << n << "[ label = \"" << lbl << "\" " << attr << "];"
     << std::endl;
}

template <typename C>
std::set<Method> GetMethods(
    const LCOM::Class<C, Method, Attribute>& LCOMInput) {
  std::set<Method> methods;
  for (const auto& method : LCOMInput.methods) {
    methods.insert(method.GetId());
  }
  return methods;
}

// An attribute node used to form a tree.
struct ANode {
  // The ID of the attribute.
  const Attribute::T id;
  // The parent node.
  const ANode* parent = nullptr;
  // The name of the attribute.
  const std::string name;
  // Methods that access this specific attribute.
  std::set<Method> methods;
  // Increasingly specialized fields associated with an attribute.
  std::map<Attribute::T, ANode*> fields;

  ANode(const Attribute::T id, const ANode* parent = nullptr,
        const std::string name = "")
      : id(id), parent(parent), name(name) {}
  // Recursively free the tree.
  ~ANode() {
    for (auto& field : fields) {
      delete field.second;
    }
  }

  const ANode* GetNonCluster() const {
    for (const auto& field : fields) {
      // Found a non-cluster.
      // This is a leaf node because there are no child fields beneath it.
      if (!field.second->fields.size()) return field.second;
      // Recursively search for some non-cluster.
      const ANode* ret = field.second->GetNonCluster();
      if (ret) return ret;
    }
    return nullptr;
  }

  // Get the full pointer name from the tree.
  std::string PName() const {
    if (id == nullptr) return "p";
    std::stringstream ss;
    ss << parent->PName() << "_" << size_t(id);
    return ss.str();
  }
};

template <typename C>
ANode* GetTree(const LCOM::Class<C, Method, Attribute>& LCOMInput) {
  ANode* root = new ANode(nullptr);
  if (!root) LOG(FATAL) << "Failed to allocate the root node." << std::endl;
  for (const auto& method : LCOMInput.methods) {
    for (const auto& a : method.attributes) {
      const Traverse::AType& aType = a.GetId();
      //  Build up each attribute within the tree.
      ANode* curr = root;
      for (auto it = aType.cbegin(); it != aType.cend(); ++it) {
        std::string name = (*it)->unparseToString();
        if (anonymous) {
          std::hash<std::string> hasher;
          name = std::to_string(hasher(name));
        }
        curr =
            curr->fields.emplace(*it, new ANode(*it, curr, name)).first->second;
        if (!curr)
          LOG(FATAL) << "Failed to allocate a node for " << name << "."
                     << std::endl;
      }
      curr->methods.emplace(method.GetId());
    }
  }
  return root;
}

void PrintTreeAttributes(std::ostream& os, const ANode* a) {
  if (!a) return;
  // Special case for root node.
  if (!a->id) {
    for (const auto& field : a->fields) {
      PrintTreeAttributes(os, field.second);
    }
    return;
  }
  // Special case for clusters.
  if (a->fields.size() > 0) {
    os << "  subgraph cluster_" << a->PName() << " {" << std::endl;
    os << "  label=\"" << a->name << "\"" << std::endl;
    for (const auto& field : a->fields) {
      PrintTreeAttributes(os, field.second);
    }
    os << "  }" << std::endl;
  } else {
    Node(os, a->PName(), a->name, "shape=ellipse");
  }
  return;
}

void PrintMethods(std::ostream& os, const std::set<Method> methods) {
  for (const auto& method : methods) {
    if (!method)
      LOG(FATAL) << "Unable to resolve method " << NPrint::p(method)
                 << std::endl;
    std::string name = ((method == nullptr) ? "null" : method->get_name());
    if (anonymous) {
      std::hash<std::string> hasher;
      name = std::to_string(hasher(name));
    }
    Node(os, PName(method), name, "shape=polygon");
  }
}

void PrintConnections(std::ostream& os, const ANode* a) {
  if (!a) return;
  const ANode* idNode = a;
  std::stringstream attribute;
  // If this is a cluster, we must instead point to something within it that is
  // not a cluster.
  if (a->fields.size()) {
    idNode = a->GetNonCluster();
    attribute << "lhead=cluster_" << a->PName();
  }
  for (const auto& method : a->methods) {
    const SgNode* mNode = isSgNode(method);
    Edge(os, PName(mNode), idNode->PName(), "", attribute.str());
  }
  for (const auto& field : a->fields) {
    PrintConnections(os, field.second);
  }
  return;
}

template <typename C>
void printMethodConnections(
    std::ostream& os, const LCOM::Class<C, Method, Attribute>& LCOMInput) {
  for (const auto& method : LCOMInput.methods) {
    for (const auto& calledMethod : method.calledMethods) {
      Edge(os, PName(method.GetId()), PName(calledMethod.GetId()), "", "");
    }
  }
}

// Ensure nodes are properly grouped at the same level.
void GroupNodes(std::ostream& os, const std::set<SgNode*> map) {
  os << "  { rank=same; ";
  for (const auto& n : map) {
    os << PName(n) << "; ";
  }
  os << "}" << std::endl;
}

// Convert LCOM graph data into a DOT graph.
template <typename C>
void LCOMToDOT(std::ostream& os,
               const LCOM::Class<C, Method, Attribute>& LCOMInput) {
  Header(os);
  const ANode* tree = GetTree(LCOMInput);
  std::set<Method> methods = GetMethods(LCOMInput);
  PrintTreeAttributes(os, tree);
  PrintMethods(os, methods);
  PrintConnections(os, tree);
  printMethodConnections(os, LCOMInput);
  delete tree;
  std::set<SgNode*> methodNodes = [&methods]() -> auto {
    std::set<SgNode*> nodes;
    for (const auto& method : methods) {
      nodes.insert(is<SgNode>(method));
    }
    return nodes;
  }();
  GroupNodes(os, methodNodes);
  Footer(os);
}

int main(int argc, char* argv[]) {
  ROSE_INITIALIZE;
  std::vector<std::string> cmdLineArgs{argv + 1, argv + argc};
  Settings settings;
  std::tie(cmdLineArgs, settings) =
      parseArgs(std::move(cmdLineArgs), settings, argc, argv);

  if (anonymous) {
    LOG(DEBUG) << "Remaining args:";
    for (auto i = cmdLineArgs.begin(); i != cmdLineArgs.end(); ++i) {
      LOG(DEBUG) << *i << ' ';
    }
    LOG(DEBUG) << std::endl;
  }

  // GetProject expects to see the name of the executable as the first argument.
  // Restore it.
  cmdLineArgs.insert(cmdLineArgs.begin(), argv[0]);
  SgProject* project = Traverse::GetProject(cmdLineArgs);
  LOG(DEBUG) << "Running analysis for class type: " << settings.classType
             << std::endl;
  switch (settings.classType) {
    case ClassType::Package:
      GenerateLCOMGraphs<SgAdaPackageSpec*>(project, settings);
      break;
    case ClassType::Function:
      GenerateLCOMGraphs<SgFunctionDeclaration*>(project, settings);
      break;
    case ClassType::Class:
      GenerateLCOMGraphs<SgClassDeclaration*>(project, settings);
      break;
    case ClassType::ProtectedObject:
      GenerateLCOMGraphs<SgAdaProtectedSpec*>(project, settings);
      break;
    case ClassType::Namespace:
      GenerateLCOMGraphs<SgNamespaceDeclarationStatement*>(project, settings);
      break;
    case ClassType::Default:
      LOG(INFO) << "No/invalid class type specified. Running analysis on "
                   "default type, "
                << typeid(Class).name() << "." << std::endl;
      GenerateLCOMGraphs<Class>(project, settings);
      break;
    case ClassType::All:
      GenerateLCOMGraphs<SgAdaPackageSpec*>(project, settings);
      GenerateLCOMGraphs<SgFunctionDeclaration*>(project, settings);
      GenerateLCOMGraphs<SgClassDeclaration*>(project, settings);
      GenerateLCOMGraphs<SgAdaProtectedSpec*>(project, settings);
      GenerateLCOMGraphs<SgNamespaceDeclarationStatement*>(project, settings);
  }

  return 0;
}
