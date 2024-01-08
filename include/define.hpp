#ifndef DEFINE_HPP
#define DEFINE_HPP

// Global variables and data types.

#include "aixlog.hpp"

// The default class type.
// This class type is used if one isn't explicitly set by command line argument.
using Class = SgAdaPackageSpec*;
// using Class = SgFunctionDeclaration*;
// using Class = SgClassDeclaration*;
// using Class = SgAdaProtectedSpec*;

enum class DotBehavior {
  LeftOnly,  // Any access to the left or right part of a dot operator is viewed
             // as an access to only the left part.
  Full       // Any access to the right part of the dot operator is viewed as an
             // access to the right part, but it is disambiguated from other
  // instances of the record using the left part. Any access to the left part is
  // also viewed as an overlapping access to any right parts contained within.
};
std::ostream& operator<<(std::ostream& os, const DotBehavior& d) {
  switch (d) {
    case DotBehavior::LeftOnly:
      os << "LeftOnly";
      break;
    case DotBehavior::Full:
      os << "Full";
      break;
    default:
      LOG(FATAL) << "Attempted to print unspecified DotBehavior" << std::endl;
  }
  return os;
}

// The supported class types.
// Other node types are likely to work, but have not been tested.
enum class ClassType {
  Default,
  Package,
  Function,
  Class,
  ProtectedObject,
  All
};
std::ostream& operator<<(std::ostream& os, const ClassType& c) {
  switch (c) {
    case ClassType::Default:
      os << "Default";
      break;
    case ClassType::Package:
      os << "Package";
      break;
    case ClassType::Function:
      os << "Function";
      break;
    case ClassType::Class:
      os << "Class";
      break;
    case ClassType::ProtectedObject:
      os << "ProtectedObject";
      break;
    case ClassType::All:
      os << "All";
      break;
    default:
      LOG(FATAL) << "Attempted to print unspecified ClassType" << std::endl;
  }
  return os;
}

// Global options set via command line.
AixLog::Severity debug = AixLog::Severity::fatal;
bool anonymous = false;
DotBehavior dotBehavior = DotBehavior::LeftOnly;

#endif  // DEFINE_HPP