#ifndef NODE_PRINT_HPP
#define NODE_PRINT_HPP

#include <sstream>
#include <string>

#include "sageGeneric.h"
// May need to define Sage Interface first to avoid conflicts.
#include "define.hpp"
#include "sageInterface.h"

namespace NPrint {

namespace sb = SageBuilder;
namespace si = SageInterface;

// TODO: As a long term fix, try to figure out a name resolution approach that
// works with C++14 and automatically picks the best function available for a
// given node type.

struct NPrint : sg::DispatchHandler<std::string> {
  // Default print handler.
  template <typename T>
  auto get_print_name_impl(const T& n) {
    return si::get_name(&n);
  }

  // Other ways to handle printing.
  // If you wish to fully enumerate them for each node type, trying these is a
  // good starting point.

  // template <typename T>
  // auto get_print_name_impl(const T& n) {
  //   return n.get_name();
  // }

  // template <typename T>
  // auto get_print_name_impl(const T& n) {
  //   return n.get_symbol() ? n.get_symbol()->get_name() : std::string();
  // }

  // template <typename T>
  // auto get_print_name_impl(const T& n) {
  //   return n.unparseToString();
  // }

  // template <typename T>
  // auto get_print_name_impl(const T& n) {
  //   return n.get_qualified_name();
  // }

  // template <typename T>
  // auto get_print_name_impl(const T& n) {
  //   return n.get_mangled_name();
  // }

  template <class T>
  std::string handle(const T& n) {
    res = get_print_name_impl(n);
    return res;
  }
};

template <>
auto NPrint::get_print_name_impl(const SgClassDeclaration& n) {
  return n.get_name();
}

template <>
auto NPrint::get_print_name_impl(const SgInitializedName& n) {
  return n.unparseToString();
}

template <>
auto NPrint::get_print_name_impl(const SgFunctionDeclaration& n) {
  return n.unparseToString();
}

template <>
auto NPrint::get_print_name_impl(const SgAdaPackageSpec& n) {
  return n.get_qualified_name();
}

template <>
auto NPrint::get_print_name_impl(const SgAdaProtectedSpec& n) {
  return n.get_qualified_name();
}

std::string print(const SgNode* n) {
  std::stringstream ss;
  ss << n;
  if (n == nullptr) return ss.str();
  if (!anonymous) {
    std::string dispatch = sg::dispatch(NPrint{}, n);
    ss << " (" << dispatch << ")";
  }
  ss << " (" << n->class_name() << ")";
  return ss.str();
}

std::string simple_name(const SgNode* n) {
  if (n == nullptr) return "<null>";

  std::stringstream ss;

  if (anonymous)
  {
    ss << n;
    return ss.str();
  }

  return sg::dispatch(NPrint{}, n);
}


#define p(n) print(n)

}  // namespace NPrint

#endif  // NODE_PRINT_HPP
