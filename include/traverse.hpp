#ifndef TRAVERSE_HPP
#define TRAVERSE_HPP

#include "rose.h"
// ROSE must always be included first.

#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <exception>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include "aixlog.hpp"
#include "define.hpp"
#include "is-type-rose.hpp"
#include "lcom.hpp"
#include "node-print.hpp"
#include "sageInterfaceAda.h"

namespace Traverse {

// Global sourceFile. We would like this to be set per-file, but this is a quick
// hack to make it work everywhere.
boost::filesystem::path sourceFile = boost::filesystem::path("./NoFile");

// Forward declarations.
template <typename C>
class Class;
template <typename C>
class Method;
template <typename C>
class Attribute;
template <typename C>
class CalledMethod;

namespace sagehelper
{
  // replace with SageInterface::Ada::declOf
  //   when available
  inline
  SgInitializedName& declOf(const SgEnumVal& n)
  {
    SgEnumDeclaration&        dcl = SG_DEREF(n.get_declaration());
    SgInitializedNamePtrList& lst = dcl.get_enumerators();

    const auto lim = lst.end();
    const auto pos = std::find_if( lst.begin(), lim,
                                   [&n](sg::NotNull<SgInitializedName> nm)->bool
                                   {
                                     return boost::iequals( nm->get_name().getString(),
                                                            n.get_name().getString()
                                                          );
                                   }
                                 );

    ASSERT_require(pos != lim);
    return SG_DEREF(*pos);
  }
}

// Attribute type.
class AType {
 public:
  using T = SgInitializedName*;

 private:
  std::vector<T> ids;

 public:
  // Converts a list of generic SgExpression to the attribute type by getting
  // its declaration. This is done to properly leverage the type system.
  static std::vector<T> ToAttrList(const std::vector<SgExpression*>& exps) {
    std::vector<T> attrs(exps.size());
    std::transform(exps.cbegin(), exps.cend(), attrs.begin(),
                   [](SgExpression* exp) {
                     T d = nullptr;

                     if (SgVarRefExp* vre = is<SgVarRefExp*>(exp))
                     {
                       d = is<T>(vre->get_symbol()->get_declaration());
                     }
                     else if (SgEnumVal* enm = is<SgEnumVal*>(exp))
                     {
                       d = &sagehelper::declOf(*enm);
                     }
                     else
                     {
                       if (false)
                       {
                         ASSERT_require(exp);
                         std::cerr << exp->get_parent()->unparseToString()
                                   << " -> " << exp->unparseToString()
                                   << " : " << typeid(*exp).name()
                                   << std::endl;
                       }

                       LOG(FATAL) << "Conversion of " << NPrint::p(exp)
                                  << " to SgVarRefExp* failed" << std::endl;
                     }

                     if (!d)
                       LOG(FATAL) << "Declaration of " << exp->unparseToString()
                                  << " is null." << std::endl;
                     return d;
                   });
    return attrs;
  }

  AType(std::vector<T> ids) : ids(ids) {
    if (ids.size() == 0)
      LOG(FATAL) << "Empty vector passed to AType constructor." << std::endl;
  }
  AType(T id) : ids({id}) {}
  AType(std::vector<SgExpression*> exps) {
    ids = ToAttrList(exps);
    if (ids.size() == 0)
      LOG(FATAL) << "Empty vector passed to AType constructor." << std::endl;
  }

  // Vector iterators.
  auto cbegin() const { return ids.cbegin(); }
  auto cend() const { return ids.cend(); }
  auto crbegin() const { return ids.crbegin(); }
  auto crend() const { return ids.crend(); }

  friend std::ostream& operator<<(std::ostream& os, const AType& a) {
    for (auto it = a.cbegin(); it != a.cend(); ++it) {
      os << NPrint::p(*it);
      if (std::next(it) != a.cend()) os << '-';
    }
    return os;
  }
  // NOTE: Might this be dangerous? Perhaps this could disallow finding or
  // inserting keys in some maps because they will be different but evaluate to
  // equal. It may be better to make this its own function and evaluate ATypes
  // for this sort of equality only when checking for access overlap.
  friend bool operator==(const AType& lhs, const AType& rhs) {
    const auto size = std::min(lhs.ids.size(), rhs.ids.size());
    for (std::size_t i = 0; i < size; i++) {
      if (lhs.ids[i] != rhs.ids[i]) {
        return false;
      }
    }
    return true;
  }
  friend bool operator<(const AType& lhs, const AType& rhs) {
    if (lhs.ids.size() < rhs.ids.size()) {
      return true;
    } else if (lhs.ids.size() > rhs.ids.size()) {
      return false;
    }
    for (std::size_t i = 0; i < lhs.ids.size(); i++) {
      if (lhs.ids[i] == rhs.ids[i]) {
        continue;
      } else if (lhs.ids[i] < rhs.ids[i]) {
        return true;
      } else if (lhs.ids[i] > rhs.ids[i]) {
        return false;
      }
    }
    return false;
  }
  T operator[](std::size_t idx) const { return ids[idx]; }
  // Get the root ID.
  T GetId() const { return ids[0]; }
  std::vector<T> GetIds() const { return ids; }
};

// Attributes don't use an alias. Instead, they use a custom class.
// Using AType = std::vector<SgInitializedName*>;

// MType is an alias for SgFunctionDeclaration, which covers functions and
// procedures.
using MType = SgFunctionDeclaration*;

// Class don't use an alias. Instead, it is specified using templates.
// using C = SgAdaPackageSpec*;

// Compare two vectors for matching contents, even if reordered.
template <class T>
bool compareVectors(std::vector<T> a, std::vector<T> b) {
  if (a.size() != b.size()) {
    return false;
  }
  std::sort(a.begin(), a.end());
  std::sort(b.begin(), b.end());
  return (a == b);
}

// NOTE: We use a recursive call for the sake of the type system. Intermediate
// results may not be the right node type, but the final one always should be.
// TODO: Peter: Check to see if some of these special cases should be handled by
// SageInterface::Ada::logicalParentScope().
template <typename T>
static SgNode* GetScopeRecurse(SgNode* n, SgNode* orig) {
  if (n == nullptr) {
    LOG(TRACE) << "n was null. No scope found." << std::endl;
    return nullptr;
  }

  // Make sure to get the first non-defining declaration.
  // Otherwise, multiple instances of one method may be seen as distinct.
  if (SgFunctionDeclaration* fd = is<SgFunctionDeclaration>(n)) {
    if (fd != fd->get_firstNondefiningDeclaration()) {
      LOG(TRACE) << "n was an SgFunctionDeclaration. Traversing up to "
                 << NPrint::p(fd->get_firstNondefiningDeclaration())
                 << std::endl;
      return GetScopeRecurse<T>(fd->get_firstNondefiningDeclaration(), orig);
    }
  }

  // Make sure the target isn't the same node as the original starting node.
  if (n != orig) {
    // Found the target type as a parent scope.
    if (T t = is<T>(n)) {
      LOG(TRACE) << "Found scope: " << NPrint::p(t) << std::endl;
      return n;
    }
  }

  // Reached the top. Prevent further traversal.
  if (is<SgGlobal>(n)) {
    LOG(TRACE) << "Found SgGlobal before finding requested scope type."
               << std::endl;
    return nullptr;
  }

  // Always go to the declaration from the definition.
  // Otherwise scope recursion risks skipping it.
  // TODO: Peter: Is there a generic way to check if a node is a definition,
  // then to generically get the associated declaration?
  if (SgClassDefinition* cd = is<SgClassDefinition>(n)) {
    LOG(TRACE) << "n was an SgClassDefinition. Traversing up to "
               << NPrint::p(cd->get_declaration()) << std::endl;
    return GetScopeRecurse<T>(cd->get_declaration(), orig);
  }
  if (SgFunctionDefinition* fd = is<SgFunctionDefinition>(n)) {
    LOG(TRACE) << "n was an SgFunctionDefinition. Traversing up to "
               << NPrint::p(fd->get_declaration()) << std::endl;
    return GetScopeRecurse<T>(fd->get_declaration(), orig);
  }
  if (SgAdaUnitRefExp* aure = is<SgAdaUnitRefExp>(n)) {
    LOG(TRACE) << "n was an SgAdaUnitRefExp. Traversing up to "
               << NPrint::p(aure->get_decl()) << std::endl;
    return GetScopeRecurse<T>(aure->get_decl(), orig);
  }

  // Follow the scope pointer.
  if (SgScopeStatement* s = is<SgScopeStatement>(n)) {
    // TODO: Is there a better approach here than const_cast?
    auto scope = const_cast<SgScopeStatement*>(
        SageInterface::Ada::logicalParentScope(s));
    LOG(TRACE) << "n was an SgScopeStatement. Traversing up to "
               << NPrint::p(scope) << std::endl;
    return GetScopeRecurse<T>(scope, orig);
  }

  // NOTE: This was found to provide inaccurate scope traversals. Disabled.
  // if (SgStatement* s = is<SgStatement>(n)) {
  //   LOG(TRACE) << "n was an SgStatement. Traversing up to "
  //              << NPrint::p(s->get_scope()) << std::endl;
  //   return GetScopeRecurse<T>(s->get_scope(),orig);
  // }

  LOG(TRACE) << "No scope pointer found. Checking parent "
             << NPrint::p(n->get_parent()) << std::endl;
  return GetScopeRecurse<T>(n->get_parent(), orig);
}
template <typename T>
static T GetScope(SgNode* n) {
  LOG(TRACE) << "Getting " << typeid(T).name() << " scope for " << NPrint::p(n)
             << std::endl;
  return is<T>(GetScopeRecurse<T>(n, n));
}
// We support vector types in case we need to get the scope of an elaborate
// dot expression. This is just a convenience function to avoid explicitly
// grabbing the first element in the vector.
template <typename T>
static T GetScope(AType n) {
  LOG(TRACE) << "Getting " << typeid(T).name() << " scope for " << n
             << std::endl;
  if (!n.GetId()) LOG(FATAL) << "n.GetId() was null for " << n << std::endl;
  return is<T>(GetScopeRecurse<T>(n.GetId(), n.GetId()));
}

template <typename C>
std::vector<C> GetRecords(SgNode* n) {
  namespace siada = SageInterface::Ada;

  LOG(DEBUG) << "Finding additional records for " << NPrint::p(n) << std::endl;

  std::vector<C> owningClassIds;

  // functor to store all type declarations associated with some function
  auto storeRecordAssociation =
       [&owningClassIds,n]
       (const SgDeclarationStatement* tydcl) -> void
       {
         ASSERT_require(tydcl);

         SgDeclarationStatement* typeDeclaration = tydcl->get_firstNondefiningDeclaration();

         if (C classId = is<C>(typeDeclaration)) {
            LOG(DEBUG) << NPrint::p(n) << " is a method within the class, "
                       << NPrint::p(classId) << std::endl;
            owningClassIds.push_back(classId);
         }
       };

  // Tagged records approach.
  // Functions/procedures that are tied to a tagged record will list the tagged
  // record as a parameter. This code will find the classes associated with
  // those parameters.
  if (SgFunctionDeclaration* fd = is<SgFunctionDeclaration>(n)) {
#if NEW_SIGNATURE_PROCESSING
    siada::PrimitiveSignatureElementsDesc elements = siada::primitiveSignatureElements(fd);

    if (elements.result())
      storeRecordAssociation(elements.result());

    for (auto& record : elements.parameters()) {
      // Get the type declaration of this record.
      // TODO: There seem to be some dupes. Ask Peter about it.
      storeRecordAssociation(record.typeDeclaration());
    }
#else /* NEW_SIGNATURE_PROCESSING */
    for (auto& record : siada::primitiveParameterPositions(fd)) {
      // Get the type declaration of this record.
      // TODO: There seem to be some dupes. Ask Peter about it.
      storeRecordAssociation(record.typeDeclaration());
    }
#endif /* NEW_SIGNATURE_PROCESSING */
  }
  LOG(DEBUG) << "Found " << owningClassIds.size() << " additional records for "
             << NPrint::p(n) << std::endl;
  return owningClassIds;
}

// Helper function to handle the conditional logic
template <typename C>
void GetClassIdsHelper(SgNode* n, std::vector<C>& owningClassIds,
                       std::true_type) {
  std::vector<C> recordClassIds = GetRecords<C>(n);
  owningClassIds.insert(owningClassIds.end(), recordClassIds.begin(),
                        recordClassIds.end());
}

// Overload for when C is not an SgClassDeclaration*
template <typename C>
void GetClassIdsHelper(SgNode* n, std::vector<C>& owningClassIds,
                       std::false_type) {
  LOG(DEBUG) << "No additional records to add." << std::endl;
  // Do nothing.
  return;
}

// SageInterfaceAda PrimitiveParameterDesc
// Checks for a type defined at the same package level.
// Gives position and name of parameter.
// Could be one of several types. Would need to test type as record type and
// check if record type is tagged. SgInitializedName->GetType can find it is
// record type. Can check if record declaration is tagged.
// TODO: Important to disallow duplciates?
// TODO: Need to make this a part of the search for parent classes.
template <typename C>
std::vector<C> GetClassIds(SgNode* n) {
  LOG(TRACE) << "Getting class IDs for " << NPrint::p(n) << std::endl;
  std::vector<C> owningClassIds;

  // Standard way of getting a class ID.
  // Traverse up the scope until we find the class containing this node.
  const C owningClassId = GetScope<C>(n);
  if (owningClassId != nullptr) {
    LOG(TRACE) << "Found a base class, " << NPrint::p(owningClassId)
               << std::endl;
    owningClassIds.push_back(owningClassId);
  }

  GetClassIdsHelper(n, owningClassIds, std::is_same<C, SgClassDeclaration*>());

  return owningClassIds;
}

bool IsClassOwner(SgExpression* id, const MType& mId) {
  const std::vector<SgClassDeclaration*> owningClassIds =
      GetRecords<SgClassDeclaration*>(mId);
  // Peter: use sageinterface ada type of expression instead of get_type
  // Peter: Could cast type to a SgClassType or SgNamedType, which will have a
  // good get_declaration method.
  SgDeclarationStatement* declPeter =
      is<SgClassType>(SageInterface::Ada::typeOfExpr(id).typerep())
          ->get_declaration();
  SgDeclarationStatement* decl = id->get_type()->getAssociatedDeclaration();
  LOG(TRACE) << "declPeter=" << declPeter << "\tdecl=" << decl << "\t"
             << (decl == declPeter) << std::endl;

  if (SgClassDeclaration* classRefId = is<SgClassDeclaration>(decl)) {
    for (const auto& classId : owningClassIds) {
      // Peter: Try pointer for first nondefining declaration instead of
      // getname.
      if (classId->get_firstNondefiningDeclaration() ==
          classRefId->get_firstNondefiningDeclaration()) {
        LOG(TRACE) << "Match found for " << NPrint::p(classRefId) << std::endl;
        return true;
      }
    }
  }
  LOG(TRACE) << "No match found for " << NPrint::p(id) << std::endl;
  return false;
}

// Class, Method, and Attribute objects are all Component types.
// They are distinguished by the use of node pointers as unique IDs.
template <typename T>
class Component {
 public:
  T id;
  Component() : id(nullptr) {}
  Component(T id) : id(id) {}
  T GetId() const { return id; }
  Component& operator=(const Component& other) {
    if (this != &other) this->id = other.id;
    return *this;
  }
};

template <typename C>
class Class : public Component<C> {
  using LCOMType = LCOM::Class<C, MType, AType>;

 public:
  std::map<MType, Method<C>*> methods;
  const boost::filesystem::path sourceFile;

 public:
  Class(const Class& t)
      : Component<C>(t.id), methods(t.methods), sourceFile(t.sourceFile) {}
  Class(C id, boost::filesystem::path sourceFile)
      : Component<C>(id), sourceFile(sourceFile) {}
  // Generate the LCOMType used in all LCOM analysis.
  LCOMType ToLCOMClass() const {
    LCOMType classLCOM = LCOMType(this->GetId());
    for (const auto& m : methods) {
      const auto& mId = std::get<0>(m);
      const auto& method = std::get<1>(m);

      LOG(TRACE) << "Adding method " << NPrint::p(mId) << " to LCOM Class "
                 << NPrint::p(this->GetId()) << std::endl;
      LCOM::Method<MType, AType> methodLCOM = LCOM::Method<MType, AType>(mId);

      for (const auto& a : method->attributes) {
        const auto& aId = std::get<0>(a);
        const auto& attribute = std::get<1>(a);

        LOG(TRACE) << "Adding attribute " << attribute << " to method "
                   << NPrint::p(mId) << " to LCOM Class "
                   << NPrint::p(this->GetId()) << std::endl;
        const auto res = methodLCOM.attributes.emplace(aId);
        // It should always succeed.
        if (!res.second)
          LOG(FATAL) << "Failed to emplace attribute " << attribute
                     << " in method " << methodLCOM << std::endl;
      }

      for (const auto& c : method->calledMethods) {
        const auto& cmId = std::get<0>(c);

        LOG(TRACE) << "Adding called method " << NPrint::p(cmId)
                   << " to method " << NPrint::p(mId) << " to LCOM Class "
                   << NPrint::p(this->GetId()) << std::endl;
        methodLCOM.calledMethods.emplace_back(cmId);
      }
      if (classLCOM.methods.count(methodLCOM) > 0) {
        LOG(TRACE) << methodLCOM << " is already in class " << classLCOM
                   << std::endl;
        continue;
      }
      auto res = classLCOM.methods.emplace(methodLCOM);
      // It should always succeed.
      if (!res.second)
        LOG(FATAL) << "Failed to emplace method " << methodLCOM << " in class "
                   << classLCOM << std::endl;
    }
    return classLCOM;
  }
  friend std::ostream& operator<<(std::ostream& os, const Class<C>& c) {
    os << NPrint::p(c.id);
    return os;
  }
  // Filter attributes for non-tagged type classes.
  void FilterAttributes(Method<C>* method, std::false_type) {
    if (!method) return;
    auto& attributes = method->attributes;
    for (auto j = attributes.begin(); j != attributes.end();) {
      const auto& attribute = std::get<1>(*j);
      // Remove foreign attributes.
      if (attribute->owningClass.GetId() != this->GetId()) {
        LOG(DEBUG) << "Removing " << attribute << " from " << *this
                   << " because " << NPrint::p(attribute->owningClass.GetId())
                   << " != " << NPrint::p(this->GetId()) << std::endl;
        j = attributes.erase(j);
      } else {
        ++j;
      }
    }
  }
  // Tagged types. Do nothing, for now.
  void FilterAttributes(Method<C>* method, std::true_type) { return; }

  // Filter out class data we don't want in the final analysis.
  void Filter() {
    LOG(DEBUG) << "Filtering out foreign data for class " << *this << std::endl;
    // The class should have no references to methods or functions contained
    // within other classes.
    for (auto i = methods.begin(); i != methods.end();) {
      const auto& method = std::get<1>(*i);

      // Remove foreign methods.
      if (method->owningClass.GetId() != this->GetId()) {
        LOG(DEBUG) << "Removing " << method << " from " << *this << " because "
                   << NPrint::p(method->owningClass.GetId())
                   << " != " << NPrint::p(this->GetId()) << std::endl;
        i = methods.erase(i);
        continue;
      } else {
        ++i;
      }

      // TODO: This is a hack. The right way to do this is to recognize that an
      // attribute could be associated with multiple classes and track them all.
      // For now, we will keep it disabled for tagged types analysis.
      FilterAttributes(method, std::is_same<C, SgClassDeclaration*>());

      auto& calledMethods = method->calledMethods;
      for (auto j = calledMethods.begin(); j != calledMethods.end();) {
        const auto& cmId = std::get<0>(*j);
        const auto& calledMethod = std::get<1>(*j);

        // Remove foreign method calls.
        if (calledMethod.owningClass.GetId() != this->GetId()) {
          LOG(DEBUG) << "Removing " << calledMethod << " from " << *this
                     << " because "
                     << NPrint::p(calledMethod.owningClass.GetId())
                     << " != " << NPrint::p(this->GetId()) << std::endl;
          j = calledMethods.erase(j);
        } else if (calledMethod.callingMethod.GetId() != method->GetId()) {
          LOG(DEBUG) << "Removing " << calledMethod << " from " << method
                     << " because "
                     << NPrint::p(calledMethod.callingMethod.GetId())
                     << " != " << NPrint::p(method->GetId()) << std::endl;
          j = calledMethods.erase(j);
        } else if (!methods.count(cmId)) {
          LOG(INFO) << "Filtering out called method " << NPrint::p(cmId)
                    << " since it is not within the class" << std::endl;
          j = calledMethods.erase(j);
        } else {
          ++j;
        }
      }
    }
  }
};
template <typename C>
class Method : public Component<MType> {
 public:
  Class<C>& owningClass;
  // Attributes accessed by the method.
  std::map<AType, Attribute<C>*> attributes;
  // Methods accessed by the method.
  // NOTE: These are their own objects, not merely references. This may be
  // unnecessary, but it does make for more flexible printouts.
  std::map<MType, CalledMethod<C>> calledMethods;

  Method(MType id, Class<C>& owningClass)
      : Component<MType>(id), owningClass(owningClass) {}
  friend std::ostream& operator<<(std::ostream& os, const Method<C>& m) {
    os << NPrint::p(m.id);
    return os;
  }
  std::string printCalledMethods() {
    std::stringstream ss;
    for (const auto& c : calledMethods) {
      const auto& calledMethod = std::get<1>(c);
      ss << calledMethod;
    }
    return ss.str();
  }
};
template <typename C>
class Attribute : public Component<AType> {
 public:
  Class<C>& owningClass;

  Attribute(AType id, Class<C>& owningClass)
      : Component<AType>(id), owningClass(owningClass) {}

  static bool IsLocalVar(const AType::T& a, const MType& baseOwningMethod) {
    if (!a)
      LOG(FATAL) << "Null attribute passed into IsLocalVar()." << std::endl;
    const MType owningMethod = GetScope<MType>(a);
    // If no method scope was found, then it was declared outside of a
    // method.
    bool isLocal = (owningMethod == baseOwningMethod);
    LOG(DEBUG) << NPrint::p(a) << " is " << (isLocal ? "" : "not ") << "local."
               << std::endl;
    return isLocal;
  }
  static bool IsLocalVar(const AType& a, const MType& baseOwningMethod) {
    return IsLocalVar(a.GetId(), baseOwningMethod);
  }

  friend std::ostream& operator<<(std::ostream& os, const Attribute<C>& a) {
    os << a.GetId();
    return os;
  }
};

template <typename C>
class CalledMethod : public Component<MType> {
 public:
  Class<C>& owningClass;
  Method<C>& callingMethod;

  CalledMethod(MType id, Class<C>& owningClass, Method<C>& callingMethod)
      : Component<MType>(id),
        owningClass(owningClass),
        callingMethod(callingMethod) {}
  friend std::ostream& operator<<(std::ostream& os, const CalledMethod<C>& m) {
    os << NPrint::p(m.id);
    return os;
  }
};

// IA = Inherited Attribute
template <typename C>
class IA {
 public:
  // These store a mapping of underlying Class, Method, and Attribute objects.
  // These can be used for lookup.
  // All other locations in the code should use references to these objects,
  // rather than making copies.
  static std::map<C, Class<C>> classData;
  static std::map<MType, Method<C>> methodData;
  static std::map<AType, Attribute<C>> attributeData;

  // Stores a mapping between renamings and their renamed attributes/methods.
  static std::map<SgAdaRenamingDecl*, AType> attributeAliasMap;
  static std::map<MType, MType> methodAliasMap;

  // Specific constructors are required to create a valid inherited attribute.
  IA(){};
  IA(const IA& X){};
  // IA(const IA& X) : class_(X.class_), Method_(X.method_){};

  // Print out all currently processed class, method, and attribute data.
  friend std::ostream& operator<<(std::ostream& os, const IA<C>& c) {
    for (const auto& c : classData) {
      const auto& classInst = std::get<1>(c);
      os << "Class: " << classInst << std::endl;
      for (const auto& m : classInst.methods) {
        const auto& method = std::get<1>(m);
        os << "\tMethod: " << *method << std::endl;
        for (const auto& a : method->attributes) {
          const auto& aId = std::get<0>(a);
          os << "\t\tAttribute:     " << aId << std::endl;
        }
        for (const auto& cm : method->calledMethods) {
          const auto& calledMethod = std::get<1>(cm);
          os << "\t\tCalled Method: " << calledMethod << std::endl;
        }
      }
    }
    return os;
  }
  static std::string printClassData() {
    std::stringstream ss;
    ss << "Contents of classData:" << std::endl;
    for (const auto& c : classData) {
      const auto& cId = std::get<0>(c);
      const auto& classInst = std::get<1>(c);
      ss << cId << ": " << classInst << std::endl;
    }
    return ss.str();
  }
  static std::string printMethodData() {
    std::stringstream ss;
    ss << "Contents of methodData:" << std::endl;
    for (const auto& m : methodData) {
      const auto& mId = std::get<0>(m);
      const auto& methodInst = std::get<1>(m);
      ss << mId << ": " << methodInst << std::endl;
    }
    return ss.str();
  }
  static std::string printAttributeData() {
    std::stringstream ss;
    ss << "Contents of attributeData:" << std::endl;
    for (const auto& a : attributeData) {
      const auto& aId = std::get<0>(a);
      const auto& attributeInst = std::get<1>(a);
      ss << aId << ": " << attributeInst << std::endl;
    }
    return ss.str();
  }
};

// Initializers for static IA data structures.
template <typename C>
std::map<C, Class<C>> IA<C>::classData = std::map<C, Class<C>>();
template <typename C>
std::map<MType, Method<C>> IA<C>::methodData = std::map<MType, Method<C>>();
template <typename C>
std::map<AType, Attribute<C>> IA<C>::attributeData =
    std::map<AType, Attribute<C>>();
template <typename C>
std::map<SgAdaRenamingDecl*, AType> IA<C>::attributeAliasMap;
template <typename C>
std::map<MType, MType> IA<C>::methodAliasMap;

// It is possible to need to go multiple layers down, alternating between
// renames, dots, pointer derefs, etc.
// NOTE: The vector stores the full resolution of record-field
// relationships.
std::vector<SgExpression*> GetRootExp(SgExpression* exp) {
  if (!exp) LOG(WARNING) << " nullptr passed into GetRootExp()." << std::endl;

  // Traverse down each attribute renaming.
  if (SgAdaRenamingRefExp* arre = is<SgAdaRenamingRefExp>(exp)) {
    LOG(TRACE) << NPrint::p(arre)
               << " is a SgAdaRenamingRefExp*. Getting what it renames."
               << std::endl;
    return GetRootExp(arre->get_decl()->get_renamed());
  }

  // Traverse down each function renaming.
  // NOTE: This situation may not actually be possible.
  if (SgAdaFunctionRenamingDecl* afrd = is<SgAdaFunctionRenamingDecl>(exp)) {
    LOG(TRACE) << NPrint::p(afrd)
               << " is a SgAdaFunctionRenamingDecl*. Getting what it renames."
               << std::endl;
    return GetRootExp(afrd->get_renamed_function());
  }

  // We can potentially rename a "dot expression" to conveniently reference a
  // specific field within a record. We currently consider the whole record to
  // be a single attribute, so we will treat this renaming as though it points
  // to the associated record instance.
  if (SgDotExp* dot = is<SgDotExp>(exp)) {
    if (dotBehavior == DotBehavior::LeftOnly) {
      return GetRootExp(dot->get_lhs_operand());
    }
    if (dotBehavior == DotBehavior::Full) {
      std::vector<SgExpression*> ret;
      auto l = GetRootExp(dot->get_lhs_operand());
      auto r = GetRootExp(dot->get_rhs_operand());
      ret.reserve(l.size() + r.size());
      ret.insert(ret.end(), l.begin(), l.end());
      ret.insert(ret.end(), r.begin(), r.end());
      return ret;
    }
    throw std::logic_error("Unimplemented dotBehavior specified");
  }

  // Traverse down pointer derefs.
  if (SgPointerDerefExp* pde = is<SgPointerDerefExp>(exp)) {
    return GetRootExp(pde->get_operand());
  }

  // Traverse down array pointer derefs.
  if (SgPntrArrRefExp* pare = is<SgPntrArrRefExp>(exp)) {
    return GetRootExp(pare->get_lhs_operand());
  }

  if (SgAdaAttributeExp* attr = is<SgAdaAttributeExp>(exp)) {
    return GetRootExp(attr->get_object());
  }

  //
  if (SgCastExp* castexp = is<SgCastExp>(exp)) {
    return GetRootExp(castexp->get_operand());
  }

  if (/*SgTypeExpression* typeex =*/ is<SgTypeExpression>(exp)) {
    return {};
  }

  // PP: 05/13/24 not sure how to handle function calls..
  //              ignore them for now?
  //     case:
  //            x : Integer renames Identity(1); -- x renames result of function call
  //                                             -- similar to variable
  if (/*SgFunctionCallExp* callexp =*/ is<SgFunctionCallExp>(exp)) {
    return {};
  }

  // No further unwrapping match found.
  return { exp };
}
SgExpression* GetBaseRootExp(std::vector<SgExpression*> rootExp) {
  if (!rootExp.size())
  {
     // PP 05/13/24 return nullptr instead of failing..
     LOG(WARNING) << "Empty root list found." << std::endl;
     return nullptr;
  }

  return rootExp[0];
}
template <typename T>
T GetBaseRootExp(SgExpression* id) {
  const std::vector<SgExpression*> rootExp = GetRootExp(is<SgExpression>(id));
  if (!rootExp.size())
    LOG(FATAL) << "Empty root list found for " << NPrint::p(id) << std::endl;

  SgExpression* root = GetBaseRootExp(rootExp);
  T t = is<T>(root);
  if (!t) {
    if (SgAdaAttributeExp* aae = is<SgAdaAttributeExp>(root)) {
      LOG(DEBUG) << "Found SgAdaAttributeExp " << NPrint::p(aae)
                 << ". Returning nullptr." << std::endl;
      return nullptr;
    }
    LOG(WARNING) << "Empty root found for " << NPrint::p(id) << std::endl;
  }
  return t;
}

template <typename C>
Class<C>* GetOwningClass(SgNode* n) {
  LOG(TRACE) << "Getting Owning Class for " << NPrint::p(n) << std::endl;
  const std::vector<C> owningClassIds = GetClassIds<C>(n);

  // TODO: Not a safe assumption, but we will see how much damage this causes.
  if (owningClassIds.size() > 1) {
    LOG(WARNING) << "owningClassIds.size() = " << owningClassIds.size()
                 << ". This method is owned by multiple classes, some of which "
                    "will be ignored!"
                 << std::endl;
  } else if (owningClassIds.size() == 0)
    return nullptr;

  const C owningClassId = owningClassIds[0];
  // The class should already be in the map.
  if (!IA<C>::classData.count(owningClassId)) {
    LOG(INFO) << "Class " << NPrint::p(owningClassId)
              << " missing from classData map. Inserting... "
              << IA<C>::printClassData() << std::endl;
    auto e = IA<C>::classData.emplace(
        owningClassId, std::move(Class<C>(owningClassId, sourceFile)));
    const bool success = std::get<1>(e);
    if (!success)
      LOG(FATAL) << "Failed to emplace " << NPrint::p(owningClassId)
                 << std::endl;
    return &std::get<0>(e)->second;
  }
  return &IA<C>::classData.at(owningClassId);
}

template <typename C>
Method<C>* GetOwningMethod(SgNode* n) {
  const MType owningMethodId = GetScope<MType>(n);
  if (owningMethodId == nullptr) return nullptr;
  // The calling method should already be in the map.
  if (!IA<C>::methodData.count(owningMethodId)) {
    LOG(INFO) << "Did not find " << NPrint::p(owningMethodId)
              << " in methodData. Perhaps the method is not within any package "
                 "we are analyzing."
              << std::endl;
    return nullptr;
  }
  return &IA<C>::methodData.at(owningMethodId);
}

template <typename C>
class VisitorTraversal : public AstTopDownProcessing<IA<C>> {
  // Gets the function renamed by afrd. If the renamed function is not already
  // in the methodAliasMap, this function will find and add it.
  static boost::optional<MType> GetRenamed(
      const SgAdaFunctionRenamingDecl* afrd) {
    // The first non-defining declaration serves as a standard ID for the map.
    MType id = is<MType>(afrd->get_firstNondefiningDeclaration());

    // If the renaming is cached in the alias map, return it.
    auto it = IA<C>::methodAliasMap.find(id);
    if (it != IA<C>::methodAliasMap.end()) {
      return it->second;
    }

    SgExpression* renamed = afrd->get_renamed_function();
    // If there is no renamed function associated with this renaming, then
    // there is not enough information to do anything here. Skip it.
    if (!renamed) {
      LOG(INFO) << "Could not find the root expression associated with this "
                   "SgAdaFunctionRenamingDecl* "
                << NPrint::p(afrd)
                << " because the base root expression is null." << std::endl;
      return boost::none;
    }

    // Check down as many levels of indirection as needed to find the root
    // function referenced by the renaming.
    const SgFunctionRefExp* fre = GetBaseRootExp<SgFunctionRefExp*>(renamed);
    if (!fre) {
      LOG(INFO) << NPrint::p(renamed) << " was an " << renamed->class_name()
                << " not an SgFunctionRefExp*." << std::endl;
      return boost::none;
    }

    // Get the declaration associated with the root expression.
    MType decl = is<MType>(fre->get_symbol()->get_declaration());

    // Store the renamed function in the alias map.
    // Use the first non-defining declaration as the key.
    bool success = IA<C>::methodAliasMap.emplace(id, decl).second;
    if (!success)
      LOG(FATAL) << "Failed to emplace method " << NPrint::p(id)
                 << " into methodAliasMap." << std::endl;

    return decl;
  }

  static IA<C> HandleMethod(const MType& pId, IA<C>& ia) {
    if (!pId)
      LOG(FATAL) << "Null method passed into HandleMethod()." << std::endl;

    // Only consider the first non-defining declaration.
    // This ensures no duplicate IDs are found.
    MType id = is<MType>(pId->get_firstNondefiningDeclaration());
    if (!id)
      LOG(FATAL) << "Null method found as firstNondefiningDeclaration."
                 << std::endl;

    Class<C>* cPtr = GetOwningClass<C>(id);
    if (!cPtr) return IA<C>(ia);
    Class<C>& owningClass = *cPtr;

    // In some cases, the same method will be seen more than once.
    // For instance, if a procedure is forward-declared.
    if (owningClass.methods.count(id)) {
      LOG(NOTICE) << NPrint::p(id) << " has already been seen. Nothing to do."
                  << std::endl;
      return IA<C>(ia);
    }

    // Handle SgAdaFunctionRenamingDecl types.
    // NOTE: Don't use first non-defining declaration here, as it may hide the
    // renamed function.
    if (SgAdaFunctionRenamingDecl* afrd = is<SgAdaFunctionRenamingDecl>(pId)) {
      const boost::optional<MType> result = GetRenamed(afrd);
      // We cannot handle a renamed function if it doesn't have a valid renamed
      // function reference.
      if (!result) return IA<C>(ia);
      MType m = *result;
      LOG(NOTICE) << NPrint::p(afrd)
                  << " is an SgAdaFunctionRenamingDecl. Associated with"
                  << NPrint::p(m) << std::endl;
      return IA<C>(ia);
    }

    // Add the method to methodData.
    bool success =
        IA<C>::methodData.emplace(id, std::move(Method<C>(id, owningClass)))
            .second;
    if (!success)
      LOG(FATAL) << "Failed to emplace method " << NPrint::p(id)
                 << " into methodData" << std::endl;

    // Add the method to owningClass methods list.
    success = owningClass.methods.emplace(id, &IA<C>::methodData.at(id)).second;
    if (!success)
      LOG(FATAL) << "Failed to emplace method " << NPrint::p(id)
                 << " into class " << owningClass << std::endl;

    LOG(DEBUG) << IA<C>::printMethodData() << std::endl;
    return IA<C>(ia);
  }

  // TODO: Actually use this to store additional classes associated with an
  // attribute.
  static bool HandleSgVarRefExpTaggedTypeHelper(Class<C>& owningClass,
                                                AType aDecl, std::true_type) {
    LOG(TRACE) << "Attempting to find all inherited classes associated with "
               << owningClass << "." << std::endl;
    // Associate the attribute with all classes in the tagged type hierarchy.
    if (SgClassDeclaration* classDeclId =
            is<SgClassDeclaration>(owningClass.GetId())) {
      SgClassDefinition* classDefId = classDeclId->get_definition();
      if (!classDefId) {
        LOG(WARNING) << "classDefId for " << NPrint::p(classDeclId)
                     << " was NULL. Ignoring." << std::endl;
        return false;
      }
      const std::vector<SgBaseClass*> inheritanceList =
          classDefId->get_inheritances();
      for (const auto& inherit : inheritanceList) {
        SgClassDeclaration* baseClassDecl = inherit->get_base_class();
        // NOTE: Currently ignoring discriminated types. We can deal with it
        // later.
        if (baseClassDecl) {
          Class<C>& baseClass = IA<C>::classData.at(baseClassDecl);
          bool success =
              IA<C>::attributeData
                  .emplace(aDecl, std::move(Attribute<C>(aDecl, baseClass)))
                  .second;
          if (!success) {
            LOG(WARNING) << "Failed to emplace." << std::endl;
          }
        }
      }
    }
    return true;
  }

  static bool HandleSgVarRefExpTaggedTypeHelper(Class<C>& owningClass,
                                                AType aDecl, std::false_type) {
    return true;
  }

  // For tagged types.
  static Class<C>* HandleSgVarRefExpGetClass(SgVarRefExp* baseRootExp,
                                             Method<C>& owningMethod,
                                             std::true_type) {
    return &owningMethod.owningClass;
  }
  // For everything else.
  static Class<C>* HandleSgVarRefExpGetClass(SgVarRefExp* baseRootExp,
                                             Method<C>& owningMethod,
                                             std::false_type) {
    return GetOwningClass<C>(baseRootExp);
  }

  static IA<C> HandleSgVarRefExp(SgExpression* id, IA<C> ia) {
    // Get the root expression. This resolves renamings, fields, and pointers.
    std::vector<SgExpression*> root = GetRootExp(id);
    SgVarRefExp* baseRootExp = is<SgVarRefExp>(GetBaseRootExp(root));

    // PP 05/13/24 added null test
    if (baseRootExp == nullptr) return IA<C>(ia);

    Method<C>* mPtr = GetOwningMethod<C>(baseRootExp);
    if (!mPtr) return IA<C>(ia);
    Method<C>& owningMethod = *mPtr;

    // Class<C>* cPtr = GetOwningClass<C>(baseRootExp);
    Class<C>* cPtr = HandleSgVarRefExpGetClass(
        baseRootExp, owningMethod, std::is_same<C, SgClassDeclaration*>());
    if (!cPtr) return IA<C>(ia);
    Class<C>& owningClass = *cPtr;

    std::vector<AType::T> decl = AType::ToAttrList(root);

    // Filter out attributes that are declared locally.
    if (Attribute<C>::IsLocalVar(decl, owningMethod.GetId())) {
      LOG(INFO) << NPrint::p(decl[0]) << " is local. Ignoring" << std::endl;
      return IA<C>(ia);
    }

    if (dotBehavior == DotBehavior::LeftOnly) {
      // Ignore variables on the right side of a dot expression.
      if (SgDotExp* parent = is<SgDotExp>(id->get_parent())) {
        if (parent->get_rhs_operand() == id) {
          return IA<C>(ia);
        }
      }
    } else if (dotBehavior == DotBehavior::Full) {
      if (SgDotExp* parent = is<SgDotExp>(id->get_parent())) {
        const bool isTaggedRoot =
            is<SgClassDeclaration>(owningClass.GetId()) &&
            IsClassOwner(parent->get_lhs_operand(), owningMethod.GetId());
        // Only process the bottom left leaf of a dot expression to avoid
        // processing portions of the expression multiple times.
        if (!isTaggedRoot && parent->get_lhs_operand() != id) {
          LOG(INFO)
              << NPrint::p(id)
              << " is not the leftmost part of the dot expression. "
              << "Skipping to avoid processing the expression multiple times."
              << std::endl;
          return IA<C>(ia);
        }
        // Build up a vector of all records leading to this specific field in
        // a record.
        // TODO: Figure out why tagged types repeat the same attribute ID twice
        // in the list.
        do {
          SgExpression* rhs = parent->get_rhs_operand();
          std::vector<SgExpression*> exps = GetRootExp(rhs);
          std::vector<AType::T> attrs = AType::ToAttrList(exps);
          decl.insert(std::end(decl), std::begin(attrs), std::end(attrs));
        } while ((parent = is<SgDotExp>(parent->get_parent())));

        // Make sure we do not "overspecify" and end up with a chain too
        // long, that goes above the scope of the record class we are
        // currently evaluating when C is a record type.
      }
    }
    // Convert the decl to a proper AType.
    AType aDecl(decl);

    // Make sure the associated attribute exists in attributeData.
    bool success =
        IA<C>::attributeData
            .emplace(aDecl, std::move(Attribute<C>(aDecl, owningClass)))
            .second;
    if (!success)
      LOG(DEBUG) << aDecl
                 << " already in attributeData. Likely inserted preemptively "
                    "by HandleSgAdaRenamingDecl(). This is not a problem."
                 << std::endl;

    // HandleSgVarRefExpTaggedTypeHelper(owningClass, aDecl,
    //                                   std::is_same<C,
    //                                   SgClassDeclaration*>());

    // Associate it with the calling method.
    success =
        owningMethod.attributes.emplace(aDecl, &IA<C>::attributeData.at(aDecl))
            .second;
    if (!success)
      LOG(DEBUG) << "attribute " << aDecl << " already in method "
                 << owningMethod << std::endl;

    LOG(DEBUG) << IA<C>::printAttributeData() << std::endl;
    return IA<C>(ia);
  }

  static IA<C> HandleSgFunctionRefExp(SgFunctionRefExp* id, IA<C> ia) {
    // Get the root expression. This resolves renamings, fields, and pointers.
    std::vector<SgExpression*> root = GetRootExp(id);
    SgFunctionRefExp* baseRootExp = is<SgFunctionRefExp>(GetBaseRootExp(root));

    // PP 05/13/24 added null test
    if (baseRootExp == nullptr) return IA<C>(ia);

    Class<C>* cPtr = GetOwningClass<C>(baseRootExp);
    if (!cPtr) return IA<C>(ia);
    Class<C>& owningClass = *cPtr;

    Method<C>* mPtr = GetOwningMethod<C>(baseRootExp);
    if (!mPtr) return IA<C>(ia);
    Method<C>& owningMethod = *mPtr;

    MType decl = is<MType>(baseRootExp->get_symbol()->get_declaration());
    if (!decl)
      LOG(FATAL) << "Declaration of " << NPrint::p(baseRootExp) << " is null."
                 << std::endl;

    // Renamed functions need to resolve to their roots.
    if (SgAdaFunctionRenamingDecl* afrd = is<SgAdaFunctionRenamingDecl>(decl)) {
      if (auto result = GetRenamed(afrd)) {
        decl = *result;
      } else {
        LOG(WARNING) << "Could not find the renamed function associated with "
                     << NPrint::p(decl) << std::endl;
      }
    }

    bool success = owningMethod.calledMethods
                       .emplace(decl, std::move(CalledMethod<C>(
                                          decl, owningClass, owningMethod)))
                       .second;
    if (!success)
      LOG(DEBUG) << NPrint::p(decl) << " already in owningMethod.calledMethods."
                 << std::endl;

    return IA<C>(ia);
  }

  // This function is mostly superfluous.
  // It will log unexpected expression types, but most of these are meant to be
  // ignored by the analysis anyway.
  static IA<C> HandleExpression(SgExpression*& id, IA<C> ia) {
    if (is<SgBinaryOp>(id) || is<SgValueExp>(id) || is<SgInitializer>(id) ||
        is<SgExprListExp>(id) || is<SgAdaOthersExp>(id) || is<SgRangeExp>(id) ||
        is<SgCallExpression>(id) || is<SgUnaryOp>(id) ||
        is<SgNullExpression>(id) || is<SgTypeExpression>(id) ||
        is<SgAdaUnitRefExp>(id) || is<SgActualArgumentExpression>(id) ||
        is<SgAdaAttributeExp>(id) || is<SgNewExp>(id) ||
        is<SgAdaTaskRefExp>(id) || is<SgConditionalExp>(id)) {
      // Suppressed.
      LOG(DEBUG) << "Suppressing irrelevant expression " << NPrint::p(id)
                 << " of type " << id->class_name() << std::endl;
    } else {
      LOG(INFO) << "Encountered unexpected expression " << NPrint::p(id)
                << " of type: " << id->class_name() << std::endl;
    }
    LOG(DEBUG) << IA<C>::printAttributeData() << std::endl;
    return IA<C>(ia);
  }

  static IA<C> HandleAdaRenamingRefExp(SgAdaRenamingRefExp* arre, IA<C> ia) {
    // Filter out calls where SgAdaRenamingDecl is the parent.
    if (is<SgAdaRenamingDecl>(arre->get_parent())) {
      LOG(DEBUG) << "Ignoring SgAdaRenamingRefExp* " << NPrint::p(arre)
                 << " because its parent is an SgAdaRenamingDecl*" << std::endl;
      return IA<C>(ia);
    }

    // Get the root declaration associated with the renaming.
    SgAdaRenamingDecl* ard = is<SgAdaRenamingDecl>(arre->get_decl());
    if (!ard)
      LOG(FATAL) << NPrint::p(arre) << " missing declaration" << std::endl;

    // Check to see if the renaming is in the attributeAliasMap.
    auto it = IA<C>::attributeAliasMap.find(ard);
    if (it == IA<C>::attributeAliasMap.end()) {
      LOG(DEBUG) << "Renamed expression " << NPrint::p(ard)
                 << " not found in the alias map. Variable was likely part of "
                    "some ignored class of renamings. Ignoring."
                 << std::endl;
      return IA<C>(ia);
    }
    AType a = it->second;

    Method<C>* mPtr = GetOwningMethod<C>(arre);
    if (!mPtr) return IA<C>(ia);
    Method<C>& owningMethod = *mPtr;

    // Add the call to the associated method.
    if (owningMethod.attributes.count(a) != 0) {
      LOG(DEBUG) << "attribute " << a << " already in method " << owningMethod
                 << std::endl;
      return IA<C>(ia);
    }

    // Associate the attribute with the calling method.
    bool success =
        owningMethod.attributes.emplace(a, &IA<C>::attributeData.at(a)).second;
    if (!success)
      LOG(FATAL) << "Failed to emplace attribute " << a << " into method "
                 << owningMethod << std::endl;

    return IA<C>(ia);
  }

 public:
  IA<C> evaluateInheritedAttribute(SgNode* n, IA<C> ia) {
    if (MType m = is<MType>(n)) {
      LOG(INFO) << "Handling MType " << NPrint::p(m) << std::endl;
      return HandleMethod(m, ia);
    } else if (SgAdaRenamingRefExp* arre = is<SgAdaRenamingRefExp>(n)) {
      LOG(INFO) << "Handling SgAdaRenamingRefExp " << NPrint::p(arre)
                << std::endl;
      return HandleAdaRenamingRefExp(arre, ia);
    } else if (SgVarRefExp* vre = is<SgVarRefExp>(n)) {
      LOG(INFO) << "Handling SgVarRefExp " << NPrint::p(vre) << std::endl;
      return HandleSgVarRefExp(vre, ia);
    } else if (SgFunctionRefExp* fre = is<SgFunctionRefExp>(n)) {
      LOG(INFO) << "Handling SgFunctionRefExp " << NPrint::p(vre) << std::endl;
      return HandleSgFunctionRefExp(fre, ia);
    } else if (SgExpression* e = is<SgExpression>(n)) {
      LOG(INFO) << "Handling SgExpression " << NPrint::p(e) << std::endl;
      return HandleExpression(e, ia);
    }
    return IA<C>(ia);
  }
};

template <typename C>
class RenamingTraversal : public AstTopDownProcessing<IA<C>> {
  static IA<C> HandleSourceFile(SgSourceFile*& id, IA<C>& ia) {
    sourceFile = id->get_sourceFileNameWithPath();
    // Hash the path to anonymize it.
    if (anonymous) {
      boost::filesystem::path anonymizedPath;
      for (const auto& part : sourceFile) {
        std::hash<std::string> hasher;
        anonymizedPath /= std::to_string(hasher(part.string()));
      }
      sourceFile = anonymizedPath;
    }
    LOG(INFO) << "Found a source file at " << sourceFile << std::endl;
    return IA<C>(ia);
  }
  static IA<C> HandleClass(C& c, IA<C>& ia) {
    if (!c) LOG(FATAL) << "Null class passed into HandleClass()." << std::endl;

    // We should never see multiple declarations of the same class.
    if (IA<C>::classData.count(c) != 0)
      LOG(INFO) << "Found duplicate declaration of class " << NPrint::p(c)
                << ". This is likely because it was added by "
                   "HandleSgAdaRenamingDecl()."
                << std::endl;

    // TODO: Ensure the class is within the currently analyzed file?

    IA<C>::classData.emplace(c, std::move(Class<C>(c, sourceFile)));
    LOG(DEBUG) << IA<C>::printClassData() << std::endl;
    return IA<C>(ia);
  }
  static IA<C> HandleSgAdaRenamingDecl(SgAdaRenamingDecl*& renameId,
                                       IA<C>& ia) {
    // This may not be true, but I'd like to assume that all renameIds are
    // unique.
    if (IA<C>::attributeAliasMap.count(renameId))
      LOG(FATAL) << NPrint::p(renameId)
                 << " is already in the attributeAliasMap" << std::endl;

    // Get the expression referenced by the renaming.
    SgExpression* exp = renameId->get_renamed();
    if (!exp)
      LOG(FATAL) << NPrint::p(renameId) << "'s renamed expression is null"
                 << std::endl;

    // Get to the root expressions.
    std::vector<SgExpression*> root = GetRootExp(exp);
    SgNode* baseRootExp = is<SgNode>(GetBaseRootExp(root));

    // If the root expression is an SgVoidVal or SgNullExpression, then it
    // must be part of a generic function declaration, which can be safely
    // ignored.
    // PP: 05/13/24 - added null test
    if ((baseRootExp == nullptr) || is<SgVoidVal>(baseRootExp) || is<SgNullExpression>(baseRootExp)) {
      LOG(DEBUG) << NPrint::p(baseRootExp)
                 << " is part of a generic function declaration. Ignoring"
                 << std::endl;
      return IA<C>(ia);
    }

    // If the root expression is an SgAdaUnitRefExp, then it is a package
    // renaming. We can discard these because external packages are ignored in
    // our analysis of cohesion.
    if (is<SgAdaUnitRefExp>(baseRootExp)) {
      LOG(NOTICE) << NPrint::p(baseRootExp)
                  << " is part of a package renaming. Ignoring" << std::endl;
      return IA<C>(ia);
    }

    AType a = AType(root);
    if (IA<C>::attributeData.count(a) == 0) {
      // Get the owning class for varId.
      const C owningClassId = GetScope<C>(baseRootExp);
      if (owningClassId == nullptr) {
        LOG(DEBUG) << NPrint::p(baseRootExp)
                   << " has no owning class, so its renaming by "
                   << NPrint::p(renameId) << " is ignored" << std::endl;
        return IA<C>(ia);
      }
      // The class should already be in the map.
      if (!IA<C>::classData.count(owningClassId)) {
        LOG(INFO) << "Class " << NPrint::p(owningClassId)
                  << " missing from classData map. " << IA<C>::printClassData()
                  << ". Inserting..." << std::endl;
        IA<C>::classData.emplace(
            owningClassId, std::move(Class<C>(owningClassId, sourceFile)));
        LOG(DEBUG) << IA<C>::printClassData() << std::endl;
      }
      Class<C>& owningClass = IA<C>::classData.at(owningClassId);

      // Emplace the attribute.
      bool success = IA<C>::attributeData
                         .emplace(a, std::move(Attribute<C>(a, owningClass)))
                         .second;
      if (!success) {
        LOG(FATAL) << "Failed to emplace attribute " << a
                   << " into attributeData " << IA<C>::printAttributeData()
                   << std::endl;
      }
    }

    // Store the relationship in the alias map for later use.
    IA<C>::attributeAliasMap.emplace(renameId, a);
    return IA<C>(ia);
  }

 public:
  IA<C> evaluateInheritedAttribute(SgNode* n, IA<C> ia) {
    if (SgSourceFile* sf = is<SgSourceFile>(n)) {
      LOG(INFO) << "Handling SgSourceFile " << NPrint::p(sf) << std::endl;
      return HandleSourceFile(sf, ia);
    } else if (C c = is<C>(n)) {
      LOG(INFO) << "Handling C " << NPrint::p(c) << std::endl;
      return HandleClass(c, ia);
    } else if (SgAdaRenamingDecl* ard = is<SgAdaRenamingDecl>(n)) {
      LOG(INFO) << "Handling SgAdaRenamingDecl " << NPrint::p(ard) << std::endl;
      return HandleSgAdaRenamingDecl(ard, ia);
    }
    return IA<C>(ia);
  }
};

SgProject* GetProject(std::vector<std::string> cmdLineArgs) {
  // Initialize and check compatibility.
  ROSE_INITIALIZE;
  SgProject* project = frontend(cmdLineArgs);
  if (!project)
    LOG(FATAL) << "Frontend did not return a valid SgProject." << std::endl;
  ROSE_ASSERT(project != NULL);
  return project;
}

template <typename C>
const std::vector<LCOM::Class<C, MType, AType>> GetClassData(
    SgProject*& project) {
  // The inherited attribute.
  IA<C> ia = IA<C>();

  // Start by finding attribute renamings in a first pass.
  RenamingTraversal<C> rTraversal;
  rTraversal.traverseInputFiles(project, ia);

  // Now perform the main traversal.
  VisitorTraversal<C> vTraversal;
  vTraversal.traverseInputFiles(project, ia);

  // Convert node data into a format accepted by LCOM.
  std::vector<LCOM::Class<C, MType, AType>> dataLCOM;

  auto& classData = IA<C>::classData;
  for (auto i = classData.begin(); i != classData.end();) {
    auto& classInst = std::get<1>(*i);

    // Filter out class data we don't want.
    classInst.Filter();

    if (classInst.methods.size() == 0) {
      LOG(INFO) << "Skipping empty class " << classInst << std::endl;
      i = classData.erase(i);
      continue;
    } else {
      ++i;
    }
    LOG(INFO) << "Converting " << classInst << " to LCOM format." << std::endl;
    LOG(TRACE) << classInst << std::endl;
    dataLCOM.push_back(classInst.ToLCOMClass());
    LOG(DEBUG) << dataLCOM.back() << " added to LCOM data." << std::endl;
  }
  LOG(DEBUG) << "Found " << classData.size() << " classes." << std::endl;

  // Print out the final class data.
  // We can use this to evaluate if the graph matches what we expect.
  IA<C> dummy;
  LOG(INFO) << dummy << std::endl;
  return dataLCOM;
}

}  // namespace Traverse

#endif  // TRAVERSE_HPP
