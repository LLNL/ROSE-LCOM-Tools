#ifndef LCOM_HPP
#define LCOM_HPP

// LCOM measurement class.
// Feed in node data from ROSE to these functions to compute associated LCOM
// metrics.
// This code uses templates to allow any type that supports comparison to work 
// as a class, method, or attribute.

#include <algorithm>
#include <boost/any.hpp>
#include <boost/optional.hpp>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "aixlog.hpp"
#include "define.hpp"
#include "node-print.hpp"

template <typename T>
class DisjointSet {
  // Maps an object ot its associated parent.
  std::map<T, T> parent;
  // Stores the size of a set.
  std::map<T, std::size_t> size;

 public:
  // Insert an object into our disjoint set.
  // By default, new objects are in their own set unless merged.
  void Insert(T object) {
    parent[object] = object;
    size[object] = 1;
  }
  // Find the "root" parent object associated with a set.
  T Find(T object) {
    if (parent[object] == object) {
      return object;
    }
    return Find(parent[object]);
  }
  // Merge two sets together.
  void UnionSets(T a, T b) {
    // Find the root parents associated with each.
    a = Find(a);
    b = Find(b);
    // If they are already in the same set, then there is nothing to do.
    if (a == b) {
      return;
    }
    // Set a should always be largest.
    // This is done to minimize the recursive search depth needed by Find().
    if (size[a] < size[b]) {
      std::swap(a, b);
    }
    // Assign a as b's parent.
    parent[b] = a;
    // Add b's size to a.
    size[a] += size[b];
    // Wipe out b's size since it's no longer its own set.
    size.erase(b);
  }
  // Get the number of distinct disjoint sets in this data structure.
  std::size_t GetNumSets() {
    std::size_t setCount = 0;
    for (const auto& o : parent) {
      auto& obj = std::get<0>(o);
      auto& parentObj = std::get<1>(o);

      // Only "root" objects count for this tally.
      if (obj == parentObj) {
        setCount++;
      }
    }
    return setCount;
  }
};

namespace LCOM {

template <typename V>
class Attribute {
 public:
  Attribute(const V id) : id_(id) {}
  Attribute(const Attribute& X) : id_(X.id_) {}
  bool operator==(const Attribute& other) const {
    return boost::any_cast<const V&>(id_) ==
           boost::any_cast<const V&>(other.id_);
  }
  bool operator<(const Attribute& other) const {
    return boost::any_cast<const V&>(id_) <
           boost::any_cast<const V&>(other.id_);
  }
  V const& GetId() const { return boost::any_cast<const V&>(id_); }
  friend std::ostream& operator<<(std::ostream& os, const Attribute<V>& a) {
    os << "Attribute " << NPrint::p(boost::any_cast<const V&>(a.GetId()));
    return os;
  }

 private:
  const boost::any id_;
};

template <typename U, typename V>
class Method {
 public:
  Method(const U id) : id_(id) {}
  Method(const Method& X)
      : id_(X.id_), attributes(X.attributes), calledMethods(X.calledMethods){};
  bool operator==(const Method& other) const {
    return boost::any_cast<const U&>(id_) ==
           boost::any_cast<const U&>(other.id_);
  }
  bool operator<(const Method& other) const {
    return boost::any_cast<const U&>(id_) <
           boost::any_cast<const U&>(other.id_);
  }
  U const& GetId() const { return boost::any_cast<const U&>(id_); }
  friend std::ostream& operator<<(std::ostream& os, const Method<U, V>& m) {
    os << "Method " << NPrint::p(boost::any_cast<const U&>(m.GetId()))
       << std::endl;
    if (m.attributes.size() > 0) {
      os << "\tAttributes:" << std::endl;
      for (const auto& attribute : m.attributes) {
        os << boost::any_cast<const V&>(attribute.GetId()) << std::endl;
      }
    }
    if (m.calledMethods.size() > 0) {
      os << "\tCalled Methods:" << std::endl;
      for (const auto& calledMethod : m.calledMethods) {
        os << NPrint::p(boost::any_cast<const U&>(calledMethod.GetId()))
           << std::endl;
      }
    }
    return os;
  }

 private:
  const boost::any id_;

 public:
  // All attributes referenced by this method.
  std::set<Attribute<V>> attributes;
  // All methods called by this method.
  std::vector<Method<U, V>> calledMethods;
};

template <typename T, typename U, typename V>
class Class {
 public:
  Class(const T id) : id_(id) {}
  Class(const Class& X) : id_(X.id_), methods(X.methods){};
  bool operator==(const Class& other) const {
    return boost::any_cast<const T&>(id_) ==
           boost::any_cast<const T&>(other.id_);
  }
  T const& GetId() const { return boost::any_cast<const T&>(id_); }

  // Needed to make Class copy assignable.
  Class& operator=(const Class& other) { return *this; }
  friend std::ostream& operator<<(std::ostream& os, const Class<T, U, V>& c) {
    os << "Class " << NPrint::p(boost::any_cast<const T&>(c.GetId()))
       << std::endl;
    os << "Methods:" << std::endl;
    for (const auto& method : c.methods) {
      os << method;
    }
    return os;
  }

  struct TreeNode {
    TreeNode* parent = nullptr;
    std::map<typename V::T, TreeNode*> children;
    std::set<const Method<U, V>*> methods;
    TreeNode(TreeNode* parent = nullptr) : parent(parent){};
  };

  static std::tuple<TreeNode*, std::set<TreeNode*>> GetAttributeTree(
      const Class<T, U, V>& classInput) {
    TreeNode* root = new TreeNode();
    std::set<TreeNode*> methodNodes;

    for (const auto& method : classInput.methods) {
      for (const auto& attribute : method.attributes) {
        TreeNode* curr = root;
        V a = attribute.GetId();
        for (auto it = a.cbegin(); it != a.cend(); ++it) {
          curr = curr->children.emplace(*it, new TreeNode(curr)).first->second;
        }
        // Insert the method at the end of the traversal.
        curr->methods.emplace(&method);
        methodNodes.insert(curr);
      }
    }
    return std::make_tuple(root, methodNodes);
  }

  static void FreeTree(TreeNode* root) {
    if (root == nullptr) return;
    for (auto& node : root->children) {
      FreeTree(std::get<1>(node));
    }
    delete root;
  }

  template <typename X, typename = void>
  struct has_cbegin_cend : std::false_type {};

  template <typename X>
  struct has_cbegin_cend<X, std::void_t<decltype(std::declval<X>().cbegin()),
                                        decltype(std::declval<X>().cend())>>
      : std::true_type {};

  // Used to support any arbitrary attribute type, V.
  template <typename VV = V>
  static
      typename std::enable_if<!has_cbegin_cend<VV>::value, DisjointSet<U>>::type
      GetLCOM3Set(const Class<T, U, V>& classInput) {
    const auto attributeMap = GetAttributeMap(classInput.methods);
    DisjointSet<U> set;
    // Each method gets its own set initially.
    for (const auto& method : classInput.methods) {
      set.Insert(method.GetId());
    }
    // Union any set that shares an attribute.
    for (const auto& a : attributeMap) {
      auto& methodList = std::get<1>(a);

      if (methodList.size() <= 1) {
        continue;
      }
      // Get a parent to connect everything to.
      const auto& parent = methodList.begin();
      // Make the set connections.
      for (auto it = std::next(parent); it != methodList.end(); ++it) {
        set.UnionSets((*parent).GetId(), (*it).GetId());
        LOG(DEBUG) << "Unioning " << NPrint::p((*parent).GetId()) << " and "
                   << NPrint::p((*it).GetId()) << std::endl;
      }
    }
    return set;
  }

  // Used to support V = AType.
  // Considers nested fields when DotBehavior::Full is used.
  template <typename VV = V>
  static
      typename std::enable_if<has_cbegin_cend<VV>::value, DisjointSet<U>>::type
      GetLCOM3Set(const Class<T, U, V>& classInput) {
    DisjointSet<U> set;
    // Each method gets its own set initially.
    for (const auto& method : classInput.methods) {
      set.Insert(method.GetId());
    }

    // Generate a tree to identify attribute access relationships.
    TreeNode* root;
    std::set<TreeNode*> methodNodes;
    std::tie(root, methodNodes) = GetAttributeTree(classInput);

    // Union the relevant sets.
    // For DotBehavior::Full, all accesses between the root and a methodNode are
    // shared.
    for (const auto& methodNode : methodNodes) {
      if (methodNode->methods.size() == 0)
        LOG(FATAL) << "Leaf has no methods associated with it. This should be "
                      "impossible."
                   << std::endl;
      // If the node is not a leaf, then this work is redundant.
      if (methodNode->children.size() != 0) {
        // Skip work that will be done by a leaf node instead.
        continue;
      }

      const Method<U, V>* parent = *(methodNode->methods.cbegin());
      TreeNode* curr = methodNode;
      while (curr != nullptr) {
        for (const auto& method : curr->methods) {
          if (parent != method) {
            set.UnionSets(parent->GetId(), method->GetId());
            LOG(DEBUG) << "Unioning " << NPrint::p(parent->GetId()) << " and "
                       << NPrint::p(method->GetId()) << std::endl;
          }
        }
        curr = curr->parent;
      }
    }

    // Deallocate the tree.
    FreeTree(root);

    return set;
  }

  static DisjointSet<U> LCOM3ToLCOM4Set(const Class<T, U, V>& classInput,
                                        DisjointSet<U> set) {
    // Union methods that call each other.
    for (const auto& method : classInput.methods) {
      for (const auto& calledMethod : method.calledMethods) {
        set.UnionSets(method.GetId(), calledMethod.GetId());
        LOG(DEBUG) << "Unioning " << NPrint::p(method.GetId()) << " and "
                   << NPrint::p(calledMethod.GetId()) << " due to method calls"
                   << std::endl;
      }
    }
    return set;
  }

 private:
  const boost::any id_;

 public:
  // All methods associated with this class.
  std::set<Method<U, V>> methods;
};

// Can be used by LCOM1 and LCOM2 to normalize on a range from 0-1.
inline std::size_t UniquePairs(const std::size_t n) { return n * (n - 1) / 2; }

// Constructs a mapping from attributes to methods.
// This is useful for identifying associated methods in various LCOM measures.
template <typename U, typename V>
std::map<Attribute<V>, std::set<Method<U, V>>> GetAttributeMap(
    std::set<Method<U, V>> methodSet) {
  std::map<Attribute<V>, std::set<Method<U, V>>> attributeMap;
  for (auto& method : methodSet) {
    for (const auto& attribute : method.attributes) {
      attributeMap[attribute].insert(method);
    }
  }
  return attributeMap;
}

template <typename U, typename V>
std::size_t GetNumSharedPairs(std::set<Method<U, V>> methodSet) {
  std::size_t count = 0;
  auto& m = methodSet;
  // For each pair of methods.
  for (auto imA = m.cbegin(); imA != m.cend(); ++imA) {
    for (auto imB = std::next(imA); imB != m.end(); ++imB) {
      // For each pair of attributes.
      for (auto attrA : (*imA).attributes) {
        for (auto attrB : (*imB).attributes) {
          // NOTE: When V is AType, == is overridden to make this work properly.
          if (attrA.GetId() == attrB.GetId()) {
            count++;
            goto EndAttr;
          }
        }
      }
    EndAttr:;
    }
  }
  return count;
}

// TODO: Cache is currently broken and disabled. Fix it.
template <typename T, typename U, typename V>
class Cache {
  static Cache instance;
  Cache() {}

 public:
  static Cache& inst() { return instance; }
  boost::optional<std::size_t> totalPairs;
  boost::optional<std::size_t> sharedPairs;
  boost::optional<DisjointSet<U>> set;

  template <typename F, typename D, typename... Args>
  static D& GetCachedValue(boost::optional<D>& data_member, F&& calculate,
                           Args&&... args) {
    if (!data_member) {
      LOG(DEBUG) << "Calculating value for " << typeid(D).name() << " using "
                 << typeid(F).name() << std::endl;
      data_member = calculate(std::forward<Args>(args)...);
    }
    return *data_member;
  }

  std::size_t GetTotalPairs(const Class<T, U, V>& classInput) {
    return GetCachedValue(
        totalPairs,
        [](const auto& classInput) {
          auto uniquePairs = UniquePairs(classInput.methods.size());
          LOG(DEBUG) << "uniquePairs" << uniquePairs << std::endl;
          return uniquePairs;
        },
        classInput);
  }
  std::size_t GetSharedPairs(const Class<T, U, V>& classInput) {
    return GetCachedValue(
        sharedPairs,
        [](const auto& classInput) {
          const auto numSharedPairs = GetNumSharedPairs(classInput.methods);
          LOG(DEBUG) << "numSharedPairs: " << numSharedPairs << std::endl;
          return numSharedPairs;
        },
        classInput);
  }
  DisjointSet<U> GetSet(const Class<T, U, V>& classInput) {
    return GetCachedValue(
        set,
        [](const auto& classInput) {
          auto set = Class<T, U, V>::GetLCOM3Set(classInput);
          return set;
        },
        classInput);
  }
};

template <typename T, typename U, typename V>
Cache<T, U, V> Cache<T, U, V>::instance;

struct LCOM1Data {
  int sharedPairs;
  int unsharedPairs;
  int totalPairs;
};

struct LCOM5Data {
  // Number of attribute accesses.
  int a;
  // Number of attributes.
  int l;
  // Number of methods.
  int k;
};

// These are based on definitions used here:
// https://site.iugaza.edu.ps/mroos/files/Software-Metrics1.pdf#page=57

// Number of pairs of methods that do not share attributes.
template <typename T, typename U, typename V>
std::size_t LCOM1(const Class<T, U, V>& classInput, LCOM1Data* data = nullptr) {
  const std::size_t totalPairs = UniquePairs(classInput.methods.size());
  // Cache<T, U, V>::inst().GetTotalPairs(classInput);
  const size_t sharedPairs = GetNumSharedPairs(classInput.methods);
  // Cache<T, U, V>::inst().GetSharedPairs(classInput);
  const std::size_t unsharedPairs = totalPairs - sharedPairs;
  if (data != nullptr) {
    data->sharedPairs = sharedPairs;
    data->unsharedPairs = unsharedPairs;
    data->totalPairs = totalPairs;
  }
  LOG(INFO) << "Shared pairs: " << sharedPairs << std::endl;
  LOG(INFO) << "Unshared pairs: " << unsharedPairs << std::endl;
  LOG(INFO) << "Total pairs: " << totalPairs << std::endl;
  return unsharedPairs;
}

// Number of pairs of methods that do not share attributes -
// number of pairs of methods that do share attributes.
template <typename T, typename U, typename V>
std::size_t LCOM2(const Class<T, U, V>& classInput) {
  const std::size_t totalPairs = UniquePairs(classInput.methods.size());
  // Cache<T, U, V>::inst().GetTotalPairs(classInput);
  const size_t sharedPairs = GetNumSharedPairs(classInput.methods);
  // Cache<T, U, V>::inst().GetSharedPairs(classInput);
  const std::size_t unsharedPairs = totalPairs - sharedPairs;
  if (unsharedPairs < sharedPairs) {
    return 0;
  }
  return unsharedPairs - sharedPairs;
}

// Number of connected components in the graph that represents each method as
// a node and the sharing of at least one attribute as an edge.
template <typename T, typename U, typename V>
std::size_t LCOM3(const Class<T, U, V>& classInput) {
  DisjointSet<U> set = Class<T, U, V>::GetLCOM3Set(classInput);
  // Cache<T, U, V>::inst().GetSet(classInput);
  return set.GetNumSets();
}

// Number of connected components in the graph that represents each method as
// a node and the sharing of at least one attribute as an edge. Edges also
// form when one method calls another within the class.
template <typename T, typename U, typename V>
std::size_t LCOM4(const Class<T, U, V>& classInput) {
  DisjointSet<U> set = Class<T, U, V>::LCOM3ToLCOM4Set(
      classInput, Class<T, U, V>::GetLCOM3Set(classInput));
  // Class<T, U, V>::LCOM3ToLCOM4Set(classInput, Cache<T, U,
  // V>::inst().GetSet(classInput));
  return set.GetNumSets();
}

// The sum of non-module attributes accessed by a class.
// TODO: It is unclear with DotBehavior::Full just how many accesses there are
// per record.
template <typename T, typename U, typename V>
double LCOM5(const Class<T, U, V>& classInput, LCOM5Data* data = nullptr) {
  std::set<Attribute<V>> uniqueAttributes;
  std::size_t accumulator = 0;
  for (const auto& method : classInput.methods) {
    accumulator += method.attributes.size();
    uniqueAttributes.insert(method.attributes.begin(), method.attributes.end());
  }
  // The number of attribute accesses by each method in a class.
  const double a = accumulator;
  // The total number of attributes.
  // NOTE: Ignores those that are never used.
  const double l = uniqueAttributes.size();
  // The number of methods.
  const double k = classInput.methods.size();
  if (data != nullptr) {
    data->a = a;
    data->l = l;
    data->k = k;
  }
  LOG(INFO) << "a=" << a << "\tl=" << l << "\tk=" << k << std::endl;
  LOG(INFO) << "(a-k*l)/(l-k*l) = "
            << "(" << a << "-" << k << "*" << l << ")/(" << l << "-" << k << "*"
            << l << ") = " << a - (k * l) << "/" << l - (k * l) << " = "
            << (a - k * l) / (l - k * l) << std::endl;
  // The final formula.
  return (a - k * l) / (l - k * l);
}

}  // namespace LCOM

#endif  // LCOM_HPP