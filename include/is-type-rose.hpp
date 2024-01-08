#ifndef IS_TYPE_ROSE_HPP
#define IS_TYPE_ROSE_HPP

// Enable the use of templates to access "is<T>"-style checks by ROSE.

#include <type_traits>

#include "rose.h"

// These allow use of node types to identify node pointer types.
// For instance, it makes is<SgNode>() equivalent to is<SgNode*>().
template <typename T>
T* remove_pointer_and_dereference(T* ptr) {
  return ptr;
}
template <typename T>
T* remove_pointer_and_dereference(T** ptr) {
  return *ptr;
}
template <typename T>
const T* remove_pointer_and_dereference(const T* ptr) {
  return ptr;
}
template <typename T>
const T* remove_pointer_and_dereference(const T** ptr) {
  return *ptr;
}
template <typename T,
          typename std::enable_if<std::is_pointer<T>::value, int>::type = 0>
T is(SgNode* n) {
  return is<std::remove_pointer_t<T>>(remove_pointer_and_dereference(n));
}
template <typename T, typename std::enable_if<!std::is_pointer<T>::value &&
                                                  !std::is_const<T>::value,
                                              int>::type = 0>
T* is(SgNode* n) {
  return is<T>(n);
}
template <typename T, typename std::enable_if<!std::is_pointer<T>::value &&
                                                  std::is_const<T>::value,
                                              int>::type = 0>
const T* is(SgNode* n) {
  return is<std::add_pointer_t<std::add_const_t<T>>>(n);
}

// The following is a comprehensive set of template specializations for every
// node type provided by ROSE.

template <>
SgNode* is<SgNode>(SgNode* n) {
  return isSgNode(n);
}

template <>
SgSupport* is<SgSupport>(SgNode* n) {
  return isSgSupport(n);
}

template <>
SgModifier* is<SgModifier>(SgNode* n) {
  return isSgModifier(n);
}

template <>
SgModifierNodes* is<SgModifierNodes>(SgNode* n) {
  return isSgModifierNodes(n);
}

template <>
SgConstVolatileModifier* is<SgConstVolatileModifier>(SgNode* n) {
  return isSgConstVolatileModifier(n);
}

template <>
SgStorageModifier* is<SgStorageModifier>(SgNode* n) {
  return isSgStorageModifier(n);
}

template <>
SgAccessModifier* is<SgAccessModifier>(SgNode* n) {
  return isSgAccessModifier(n);
}

template <>
SgFunctionModifier* is<SgFunctionModifier>(SgNode* n) {
  return isSgFunctionModifier(n);
}

template <>
SgUPC_AccessModifier* is<SgUPC_AccessModifier>(SgNode* n) {
  return isSgUPC_AccessModifier(n);
}

template <>
SgSpecialFunctionModifier* is<SgSpecialFunctionModifier>(SgNode* n) {
  return isSgSpecialFunctionModifier(n);
}

template <>
SgElaboratedTypeModifier* is<SgElaboratedTypeModifier>(SgNode* n) {
  return isSgElaboratedTypeModifier(n);
}

template <>
SgLinkageModifier* is<SgLinkageModifier>(SgNode* n) {
  return isSgLinkageModifier(n);
}

template <>
SgBaseClassModifier* is<SgBaseClassModifier>(SgNode* n) {
  return isSgBaseClassModifier(n);
}

template <>
SgStructureModifier* is<SgStructureModifier>(SgNode* n) {
  return isSgStructureModifier(n);
}

template <>
SgTypeModifier* is<SgTypeModifier>(SgNode* n) {
  return isSgTypeModifier(n);
}

template <>
SgDeclarationModifier* is<SgDeclarationModifier>(SgNode* n) {
  return isSgDeclarationModifier(n);
}

template <>
SgOpenclAccessModeModifier* is<SgOpenclAccessModeModifier>(SgNode* n) {
  return isSgOpenclAccessModeModifier(n);
}

template <>
SgName* is<SgName>(SgNode* n) {
  return isSgName(n);
}

template <>
SgSymbolTable* is<SgSymbolTable>(SgNode* n) {
  return isSgSymbolTable(n);
}

template <>
SgAttribute* is<SgAttribute>(SgNode* n) {
  return isSgAttribute(n);
}

template <>
SgPragma* is<SgPragma>(SgNode* n) {
  return isSgPragma(n);
}

template <>
SgBitAttribute* is<SgBitAttribute>(SgNode* n) {
  return isSgBitAttribute(n);
}

template <>
SgFuncDecl_attr* is<SgFuncDecl_attr>(SgNode* n) {
  return isSgFuncDecl_attr(n);
}

template <>
SgClassDecl_attr* is<SgClassDecl_attr>(SgNode* n) {
  return isSgClassDecl_attr(n);
}

template <>
Sg_File_Info* is<Sg_File_Info>(SgNode* n) {
  return isSg_File_Info(n);
}

template <>
SgFile* is<SgFile>(SgNode* n) {
  return isSgFile(n);
}

template <>
SgSourceFile* is<SgSourceFile>(SgNode* n) {
  return isSgSourceFile(n);
}

template <>
SgUnknownFile* is<SgUnknownFile>(SgNode* n) {
  return isSgUnknownFile(n);
}

template <>
SgProject* is<SgProject>(SgNode* n) {
  return isSgProject(n);
}

template <>
SgOptions* is<SgOptions>(SgNode* n) {
  return isSgOptions(n);
}

template <>
SgUnparse_Info* is<SgUnparse_Info>(SgNode* n) {
  return isSgUnparse_Info(n);
}

template <>
SgBaseClass* is<SgBaseClass>(SgNode* n) {
  return isSgBaseClass(n);
}

template <>
SgExpBaseClass* is<SgExpBaseClass>(SgNode* n) {
  return isSgExpBaseClass(n);
}

template <>
SgNonrealBaseClass* is<SgNonrealBaseClass>(SgNode* n) {
  return isSgNonrealBaseClass(n);
}

template <>
SgTypedefSeq* is<SgTypedefSeq>(SgNode* n) {
  return isSgTypedefSeq(n);
}

template <>
SgTemplateParameter* is<SgTemplateParameter>(SgNode* n) {
  return isSgTemplateParameter(n);
}

template <>
SgTemplateArgument* is<SgTemplateArgument>(SgNode* n) {
  return isSgTemplateArgument(n);
}

template <>
SgDirectory* is<SgDirectory>(SgNode* n) {
  return isSgDirectory(n);
}

template <>
SgFileList* is<SgFileList>(SgNode* n) {
  return isSgFileList(n);
}

template <>
SgDirectoryList* is<SgDirectoryList>(SgNode* n) {
  return isSgDirectoryList(n);
}

template <>
SgFunctionParameterTypeList* is<SgFunctionParameterTypeList>(SgNode* n) {
  return isSgFunctionParameterTypeList(n);
}

template <>
SgQualifiedName* is<SgQualifiedName>(SgNode* n) {
  return isSgQualifiedName(n);
}

template <>
SgTemplateArgumentList* is<SgTemplateArgumentList>(SgNode* n) {
  return isSgTemplateArgumentList(n);
}

template <>
SgTemplateParameterList* is<SgTemplateParameterList>(SgNode* n) {
  return isSgTemplateParameterList(n);
}

template <>
SgGraph* is<SgGraph>(SgNode* n) {
  return isSgGraph(n);
}

template <>
SgIncidenceDirectedGraph* is<SgIncidenceDirectedGraph>(SgNode* n) {
  return isSgIncidenceDirectedGraph(n);
}

template <>
SgBidirectionalGraph* is<SgBidirectionalGraph>(SgNode* n) {
  return isSgBidirectionalGraph(n);
}

template <>
SgStringKeyedBidirectionalGraph* is<SgStringKeyedBidirectionalGraph>(
    SgNode* n) {
  return isSgStringKeyedBidirectionalGraph(n);
}

template <>
SgIntKeyedBidirectionalGraph* is<SgIntKeyedBidirectionalGraph>(SgNode* n) {
  return isSgIntKeyedBidirectionalGraph(n);
}

template <>
SgIncidenceUndirectedGraph* is<SgIncidenceUndirectedGraph>(SgNode* n) {
  return isSgIncidenceUndirectedGraph(n);
}

template <>
SgGraphNode* is<SgGraphNode>(SgNode* n) {
  return isSgGraphNode(n);
}

template <>
SgGraphEdge* is<SgGraphEdge>(SgNode* n) {
  return isSgGraphEdge(n);
}

template <>
SgDirectedGraphEdge* is<SgDirectedGraphEdge>(SgNode* n) {
  return isSgDirectedGraphEdge(n);
}

template <>
SgUndirectedGraphEdge* is<SgUndirectedGraphEdge>(SgNode* n) {
  return isSgUndirectedGraphEdge(n);
}

template <>
SgGraphNodeList* is<SgGraphNodeList>(SgNode* n) {
  return isSgGraphNodeList(n);
}

template <>
SgGraphEdgeList* is<SgGraphEdgeList>(SgNode* n) {
  return isSgGraphEdgeList(n);
}

template <>
SgTypeTable* is<SgTypeTable>(SgNode* n) {
  return isSgTypeTable(n);
}

template <>
SgNameGroup* is<SgNameGroup>(SgNode* n) {
  return isSgNameGroup(n);
}

template <>
SgDimensionObject* is<SgDimensionObject>(SgNode* n) {
  return isSgDimensionObject(n);
}

template <>
SgFormatItem* is<SgFormatItem>(SgNode* n) {
  return isSgFormatItem(n);
}

template <>
SgFormatItemList* is<SgFormatItemList>(SgNode* n) {
  return isSgFormatItemList(n);
}

template <>
SgDataStatementGroup* is<SgDataStatementGroup>(SgNode* n) {
  return isSgDataStatementGroup(n);
}

template <>
SgDataStatementObject* is<SgDataStatementObject>(SgNode* n) {
  return isSgDataStatementObject(n);
}

template <>
SgIncludeFile* is<SgIncludeFile>(SgNode* n) {
  return isSgIncludeFile(n);
}

template <>
SgDataStatementValue* is<SgDataStatementValue>(SgNode* n) {
  return isSgDataStatementValue(n);
}

template <>
SgJavaImportStatementList* is<SgJavaImportStatementList>(SgNode* n) {
  return isSgJavaImportStatementList(n);
}

template <>
SgJavaClassDeclarationList* is<SgJavaClassDeclarationList>(SgNode* n) {
  return isSgJavaClassDeclarationList(n);
}

template <>
SgHeaderFileReport* is<SgHeaderFileReport>(SgNode* n) {
  return isSgHeaderFileReport(n);
}

template <>
SgType* is<SgType>(SgNode* n) {
  return isSgType(n);
}

template <>
SgTypeUnknown* is<SgTypeUnknown>(SgNode* n) {
  return isSgTypeUnknown(n);
}

template <>
SgTypeChar* is<SgTypeChar>(SgNode* n) {
  return isSgTypeChar(n);
}

template <>
SgTypeSignedChar* is<SgTypeSignedChar>(SgNode* n) {
  return isSgTypeSignedChar(n);
}

template <>
SgTypeUnsignedChar* is<SgTypeUnsignedChar>(SgNode* n) {
  return isSgTypeUnsignedChar(n);
}

template <>
SgTypeShort* is<SgTypeShort>(SgNode* n) {
  return isSgTypeShort(n);
}

template <>
SgTypeSignedShort* is<SgTypeSignedShort>(SgNode* n) {
  return isSgTypeSignedShort(n);
}

template <>
SgTypeUnsignedShort* is<SgTypeUnsignedShort>(SgNode* n) {
  return isSgTypeUnsignedShort(n);
}

template <>
SgTypeInt* is<SgTypeInt>(SgNode* n) {
  return isSgTypeInt(n);
}

template <>
SgTypeSignedInt* is<SgTypeSignedInt>(SgNode* n) {
  return isSgTypeSignedInt(n);
}

template <>
SgTypeUnsignedInt* is<SgTypeUnsignedInt>(SgNode* n) {
  return isSgTypeUnsignedInt(n);
}

template <>
SgTypeLong* is<SgTypeLong>(SgNode* n) {
  return isSgTypeLong(n);
}

template <>
SgTypeSignedLong* is<SgTypeSignedLong>(SgNode* n) {
  return isSgTypeSignedLong(n);
}

template <>
SgTypeUnsignedLong* is<SgTypeUnsignedLong>(SgNode* n) {
  return isSgTypeUnsignedLong(n);
}

template <>
SgTypeVoid* is<SgTypeVoid>(SgNode* n) {
  return isSgTypeVoid(n);
}

template <>
SgTypeGlobalVoid* is<SgTypeGlobalVoid>(SgNode* n) {
  return isSgTypeGlobalVoid(n);
}

template <>
SgTypeWchar* is<SgTypeWchar>(SgNode* n) {
  return isSgTypeWchar(n);
}

template <>
SgTypeFloat* is<SgTypeFloat>(SgNode* n) {
  return isSgTypeFloat(n);
}

template <>
SgTypeDouble* is<SgTypeDouble>(SgNode* n) {
  return isSgTypeDouble(n);
}

template <>
SgTypeLongLong* is<SgTypeLongLong>(SgNode* n) {
  return isSgTypeLongLong(n);
}

template <>
SgTypeSignedLongLong* is<SgTypeSignedLongLong>(SgNode* n) {
  return isSgTypeSignedLongLong(n);
}

template <>
SgTypeUnsignedLongLong* is<SgTypeUnsignedLongLong>(SgNode* n) {
  return isSgTypeUnsignedLongLong(n);
}

template <>
SgTypeSigned128bitInteger* is<SgTypeSigned128bitInteger>(SgNode* n) {
  return isSgTypeSigned128bitInteger(n);
}

template <>
SgTypeUnsigned128bitInteger* is<SgTypeUnsigned128bitInteger>(SgNode* n) {
  return isSgTypeUnsigned128bitInteger(n);
}

template <>
SgTypeFloat80* is<SgTypeFloat80>(SgNode* n) {
  return isSgTypeFloat80(n);
}

template <>
SgTypeLongDouble* is<SgTypeLongDouble>(SgNode* n) {
  return isSgTypeLongDouble(n);
}

template <>
SgTypeString* is<SgTypeString>(SgNode* n) {
  return isSgTypeString(n);
}

template <>
SgTypeBool* is<SgTypeBool>(SgNode* n) {
  return isSgTypeBool(n);
}

template <>
SgPointerType* is<SgPointerType>(SgNode* n) {
  return isSgPointerType(n);
}

template <>
SgPointerMemberType* is<SgPointerMemberType>(SgNode* n) {
  return isSgPointerMemberType(n);
}

template <>
SgReferenceType* is<SgReferenceType>(SgNode* n) {
  return isSgReferenceType(n);
}

template <>
SgNamedType* is<SgNamedType>(SgNode* n) {
  return isSgNamedType(n);
}

template <>
SgClassType* is<SgClassType>(SgNode* n) {
  return isSgClassType(n);
}

template <>
SgJavaParameterType* is<SgJavaParameterType>(SgNode* n) {
  return isSgJavaParameterType(n);
}

template <>
SgJovialTableType* is<SgJovialTableType>(SgNode* n) {
  return isSgJovialTableType(n);
}

template <>
SgEnumType* is<SgEnumType>(SgNode* n) {
  return isSgEnumType(n);
}

template <>
SgTypedefType* is<SgTypedefType>(SgNode* n) {
  return isSgTypedefType(n);
}

template <>
SgNonrealType* is<SgNonrealType>(SgNode* n) {
  return isSgNonrealType(n);
}

template <>
SgJavaParameterizedType* is<SgJavaParameterizedType>(SgNode* n) {
  return isSgJavaParameterizedType(n);
}

template <>
SgJavaQualifiedType* is<SgJavaQualifiedType>(SgNode* n) {
  return isSgJavaQualifiedType(n);
}

template <>
SgJavaWildcardType* is<SgJavaWildcardType>(SgNode* n) {
  return isSgJavaWildcardType(n);
}

template <>
SgAdaTaskType* is<SgAdaTaskType>(SgNode* n) {
  return isSgAdaTaskType(n);
}

template <>
SgAdaProtectedType* is<SgAdaProtectedType>(SgNode* n) {
  return isSgAdaProtectedType(n);
}

template <>
SgAdaFormalType* is<SgAdaFormalType>(SgNode* n) {
  return isSgAdaFormalType(n);
}

template <>
SgAdaDiscriminatedType* is<SgAdaDiscriminatedType>(SgNode* n) {
  return isSgAdaDiscriminatedType(n);
}

template <>
SgModifierType* is<SgModifierType>(SgNode* n) {
  return isSgModifierType(n);
}

template <>
SgFunctionType* is<SgFunctionType>(SgNode* n) {
  return isSgFunctionType(n);
}

template <>
SgMemberFunctionType* is<SgMemberFunctionType>(SgNode* n) {
  return isSgMemberFunctionType(n);
}

template <>
SgPartialFunctionType* is<SgPartialFunctionType>(SgNode* n) {
  return isSgPartialFunctionType(n);
}

template <>
SgPartialFunctionModifierType* is<SgPartialFunctionModifierType>(SgNode* n) {
  return isSgPartialFunctionModifierType(n);
}

template <>
SgArrayType* is<SgArrayType>(SgNode* n) {
  return isSgArrayType(n);
}

template <>
SgTypeEllipse* is<SgTypeEllipse>(SgNode* n) {
  return isSgTypeEllipse(n);
}

template <>
SgTemplateType* is<SgTemplateType>(SgNode* n) {
  return isSgTemplateType(n);
}

template <>
SgQualifiedNameType* is<SgQualifiedNameType>(SgNode* n) {
  return isSgQualifiedNameType(n);
}

template <>
SgTypeComplex* is<SgTypeComplex>(SgNode* n) {
  return isSgTypeComplex(n);
}

template <>
SgTypeImaginary* is<SgTypeImaginary>(SgNode* n) {
  return isSgTypeImaginary(n);
}

template <>
SgTypeDefault* is<SgTypeDefault>(SgNode* n) {
  return isSgTypeDefault(n);
}

template <>
SgTypeCAFTeam* is<SgTypeCAFTeam>(SgNode* n) {
  return isSgTypeCAFTeam(n);
}

template <>
SgTypeCrayPointer* is<SgTypeCrayPointer>(SgNode* n) {
  return isSgTypeCrayPointer(n);
}

template <>
SgTypeLabel* is<SgTypeLabel>(SgNode* n) {
  return isSgTypeLabel(n);
}

template <>
SgJavaUnionType* is<SgJavaUnionType>(SgNode* n) {
  return isSgJavaUnionType(n);
}

template <>
SgRvalueReferenceType* is<SgRvalueReferenceType>(SgNode* n) {
  return isSgRvalueReferenceType(n);
}

template <>
SgTypeNullptr* is<SgTypeNullptr>(SgNode* n) {
  return isSgTypeNullptr(n);
}

template <>
SgDeclType* is<SgDeclType>(SgNode* n) {
  return isSgDeclType(n);
}

template <>
SgTypeOfType* is<SgTypeOfType>(SgNode* n) {
  return isSgTypeOfType(n);
}

template <>
SgTypeMatrix* is<SgTypeMatrix>(SgNode* n) {
  return isSgTypeMatrix(n);
}

template <>
SgTypeTuple* is<SgTypeTuple>(SgNode* n) {
  return isSgTypeTuple(n);
}

template <>
SgTypeChar16* is<SgTypeChar16>(SgNode* n) {
  return isSgTypeChar16(n);
}

template <>
SgTypeChar32* is<SgTypeChar32>(SgNode* n) {
  return isSgTypeChar32(n);
}

template <>
SgTypeFloat128* is<SgTypeFloat128>(SgNode* n) {
  return isSgTypeFloat128(n);
}

template <>
SgTypeFixed* is<SgTypeFixed>(SgNode* n) {
  return isSgTypeFixed(n);
}

template <>
SgAutoType* is<SgAutoType>(SgNode* n) {
  return isSgAutoType(n);
}

template <>
SgAdaAccessType* is<SgAdaAccessType>(SgNode* n) {
  return isSgAdaAccessType(n);
}

template <>
SgAdaSubtype* is<SgAdaSubtype>(SgNode* n) {
  return isSgAdaSubtype(n);
}

template <>
SgAdaDiscreteType* is<SgAdaDiscreteType>(SgNode* n) {
  return isSgAdaDiscreteType(n);
}

template <>
SgAdaModularType* is<SgAdaModularType>(SgNode* n) {
  return isSgAdaModularType(n);
}

template <>
SgAdaDerivedType* is<SgAdaDerivedType>(SgNode* n) {
  return isSgAdaDerivedType(n);
}

template <>
SgAdaSubroutineType* is<SgAdaSubroutineType>(SgNode* n) {
  return isSgAdaSubroutineType(n);
}

template <>
SgJovialBitType* is<SgJovialBitType>(SgNode* n) {
  return isSgJovialBitType(n);
}

template <>
SgLocatedNode* is<SgLocatedNode>(SgNode* n) {
  return isSgLocatedNode(n);
}

template <>
SgToken* is<SgToken>(SgNode* n) {
  return isSgToken(n);
}

template <>
SgLocatedNodeSupport* is<SgLocatedNodeSupport>(SgNode* n) {
  return isSgLocatedNodeSupport(n);
}

template <>
SgCommonBlockObject* is<SgCommonBlockObject>(SgNode* n) {
  return isSgCommonBlockObject(n);
}

template <>
SgInitializedName* is<SgInitializedName>(SgNode* n) {
  return isSgInitializedName(n);
}

template <>
SgInterfaceBody* is<SgInterfaceBody>(SgNode* n) {
  return isSgInterfaceBody(n);
}

template <>
SgHeaderFileBody* is<SgHeaderFileBody>(SgNode* n) {
  return isSgHeaderFileBody(n);
}

template <>
SgRenamePair* is<SgRenamePair>(SgNode* n) {
  return isSgRenamePair(n);
}

template <>
SgJavaMemberValuePair* is<SgJavaMemberValuePair>(SgNode* n) {
  return isSgJavaMemberValuePair(n);
}

template <>
SgOmpClause* is<SgOmpClause>(SgNode* n) {
  return isSgOmpClause(n);
}

template <>
SgOmpNowaitClause* is<SgOmpNowaitClause>(SgNode* n) {
  return isSgOmpNowaitClause(n);
}

template <>
SgOmpBeginClause* is<SgOmpBeginClause>(SgNode* n) {
  return isSgOmpBeginClause(n);
}

template <>
SgOmpEndClause* is<SgOmpEndClause>(SgNode* n) {
  return isSgOmpEndClause(n);
}

template <>
SgOmpUntiedClause* is<SgOmpUntiedClause>(SgNode* n) {
  return isSgOmpUntiedClause(n);
}

template <>
SgOmpDefaultClause* is<SgOmpDefaultClause>(SgNode* n) {
  return isSgOmpDefaultClause(n);
}

template <>
SgOmpAtomicClause* is<SgOmpAtomicClause>(SgNode* n) {
  return isSgOmpAtomicClause(n);
}

template <>
SgOmpProcBindClause* is<SgOmpProcBindClause>(SgNode* n) {
  return isSgOmpProcBindClause(n);
}

template <>
SgOmpExpressionClause* is<SgOmpExpressionClause>(SgNode* n) {
  return isSgOmpExpressionClause(n);
}

template <>
SgOmpOrderedClause* is<SgOmpOrderedClause>(SgNode* n) {
  return isSgOmpOrderedClause(n);
}

template <>
SgOmpCollapseClause* is<SgOmpCollapseClause>(SgNode* n) {
  return isSgOmpCollapseClause(n);
}

template <>
SgOmpIfClause* is<SgOmpIfClause>(SgNode* n) {
  return isSgOmpIfClause(n);
}

template <>
SgOmpNumThreadsClause* is<SgOmpNumThreadsClause>(SgNode* n) {
  return isSgOmpNumThreadsClause(n);
}

template <>
SgOmpDeviceClause* is<SgOmpDeviceClause>(SgNode* n) {
  return isSgOmpDeviceClause(n);
}

template <>
SgOmpSafelenClause* is<SgOmpSafelenClause>(SgNode* n) {
  return isSgOmpSafelenClause(n);
}

template <>
SgOmpSimdlenClause* is<SgOmpSimdlenClause>(SgNode* n) {
  return isSgOmpSimdlenClause(n);
}

template <>
SgOmpFinalClause* is<SgOmpFinalClause>(SgNode* n) {
  return isSgOmpFinalClause(n);
}

template <>
SgOmpPriorityClause* is<SgOmpPriorityClause>(SgNode* n) {
  return isSgOmpPriorityClause(n);
}

template <>
SgOmpInbranchClause* is<SgOmpInbranchClause>(SgNode* n) {
  return isSgOmpInbranchClause(n);
}

template <>
SgOmpNotinbranchClause* is<SgOmpNotinbranchClause>(SgNode* n) {
  return isSgOmpNotinbranchClause(n);
}

template <>
SgOmpVariablesClause* is<SgOmpVariablesClause>(SgNode* n) {
  return isSgOmpVariablesClause(n);
}

template <>
SgOmpCopyprivateClause* is<SgOmpCopyprivateClause>(SgNode* n) {
  return isSgOmpCopyprivateClause(n);
}

template <>
SgOmpPrivateClause* is<SgOmpPrivateClause>(SgNode* n) {
  return isSgOmpPrivateClause(n);
}

template <>
SgOmpFirstprivateClause* is<SgOmpFirstprivateClause>(SgNode* n) {
  return isSgOmpFirstprivateClause(n);
}

template <>
SgOmpSharedClause* is<SgOmpSharedClause>(SgNode* n) {
  return isSgOmpSharedClause(n);
}

template <>
SgOmpCopyinClause* is<SgOmpCopyinClause>(SgNode* n) {
  return isSgOmpCopyinClause(n);
}

template <>
SgOmpLastprivateClause* is<SgOmpLastprivateClause>(SgNode* n) {
  return isSgOmpLastprivateClause(n);
}

template <>
SgOmpReductionClause* is<SgOmpReductionClause>(SgNode* n) {
  return isSgOmpReductionClause(n);
}

template <>
SgOmpMapClause* is<SgOmpMapClause>(SgNode* n) {
  return isSgOmpMapClause(n);
}

template <>
SgOmpUniformClause* is<SgOmpUniformClause>(SgNode* n) {
  return isSgOmpUniformClause(n);
}

template <>
SgOmpAlignedClause* is<SgOmpAlignedClause>(SgNode* n) {
  return isSgOmpAlignedClause(n);
}

template <>
SgOmpLinearClause* is<SgOmpLinearClause>(SgNode* n) {
  return isSgOmpLinearClause(n);
}

template <>
SgOmpDependClause* is<SgOmpDependClause>(SgNode* n) {
  return isSgOmpDependClause(n);
}

template <>
SgOmpScheduleClause* is<SgOmpScheduleClause>(SgNode* n) {
  return isSgOmpScheduleClause(n);
}

template <>
SgOmpMergeableClause* is<SgOmpMergeableClause>(SgNode* n) {
  return isSgOmpMergeableClause(n);
}

template <>
SgLambdaCapture* is<SgLambdaCapture>(SgNode* n) {
  return isSgLambdaCapture(n);
}

template <>
SgLambdaCaptureList* is<SgLambdaCaptureList>(SgNode* n) {
  return isSgLambdaCaptureList(n);
}

template <>
SgAdaTypeConstraint* is<SgAdaTypeConstraint>(SgNode* n) {
  return isSgAdaTypeConstraint(n);
}

template <>
SgAdaRangeConstraint* is<SgAdaRangeConstraint>(SgNode* n) {
  return isSgAdaRangeConstraint(n);
}

template <>
SgAdaIndexConstraint* is<SgAdaIndexConstraint>(SgNode* n) {
  return isSgAdaIndexConstraint(n);
}

template <>
SgAdaDigitsConstraint* is<SgAdaDigitsConstraint>(SgNode* n) {
  return isSgAdaDigitsConstraint(n);
}

template <>
SgAdaDeltaConstraint* is<SgAdaDeltaConstraint>(SgNode* n) {
  return isSgAdaDeltaConstraint(n);
}

template <>
SgAdaDiscriminantConstraint* is<SgAdaDiscriminantConstraint>(SgNode* n) {
  return isSgAdaDiscriminantConstraint(n);
}

template <>
SgAdaNullConstraint* is<SgAdaNullConstraint>(SgNode* n) {
  return isSgAdaNullConstraint(n);
}

template <>
SgStatement* is<SgStatement>(SgNode* n) {
  return isSgStatement(n);
}

template <>
SgScopeStatement* is<SgScopeStatement>(SgNode* n) {
  return isSgScopeStatement(n);
}

template <>
SgGlobal* is<SgGlobal>(SgNode* n) {
  return isSgGlobal(n);
}

template <>
SgBasicBlock* is<SgBasicBlock>(SgNode* n) {
  return isSgBasicBlock(n);
}

template <>
SgIfStmt* is<SgIfStmt>(SgNode* n) {
  return isSgIfStmt(n);
}

template <>
SgForStatement* is<SgForStatement>(SgNode* n) {
  return isSgForStatement(n);
}

template <>
SgFunctionDefinition* is<SgFunctionDefinition>(SgNode* n) {
  return isSgFunctionDefinition(n);
}

template <>
SgTemplateFunctionDefinition* is<SgTemplateFunctionDefinition>(SgNode* n) {
  return isSgTemplateFunctionDefinition(n);
}

template <>
SgClassDefinition* is<SgClassDefinition>(SgNode* n) {
  return isSgClassDefinition(n);
}

template <>
SgTemplateInstantiationDefn* is<SgTemplateInstantiationDefn>(SgNode* n) {
  return isSgTemplateInstantiationDefn(n);
}

template <>
SgTemplateClassDefinition* is<SgTemplateClassDefinition>(SgNode* n) {
  return isSgTemplateClassDefinition(n);
}

template <>
SgWhileStmt* is<SgWhileStmt>(SgNode* n) {
  return isSgWhileStmt(n);
}

template <>
SgDoWhileStmt* is<SgDoWhileStmt>(SgNode* n) {
  return isSgDoWhileStmt(n);
}

template <>
SgSwitchStatement* is<SgSwitchStatement>(SgNode* n) {
  return isSgSwitchStatement(n);
}

template <>
SgCatchOptionStmt* is<SgCatchOptionStmt>(SgNode* n) {
  return isSgCatchOptionStmt(n);
}

template <>
SgNamespaceDefinitionStatement* is<SgNamespaceDefinitionStatement>(SgNode* n) {
  return isSgNamespaceDefinitionStatement(n);
}

template <>
SgBlockDataStatement* is<SgBlockDataStatement>(SgNode* n) {
  return isSgBlockDataStatement(n);
}

template <>
SgAssociateStatement* is<SgAssociateStatement>(SgNode* n) {
  return isSgAssociateStatement(n);
}

template <>
SgFortranDo* is<SgFortranDo>(SgNode* n) {
  return isSgFortranDo(n);
}

template <>
SgFortranNonblockedDo* is<SgFortranNonblockedDo>(SgNode* n) {
  return isSgFortranNonblockedDo(n);
}

template <>
SgForAllStatement* is<SgForAllStatement>(SgNode* n) {
  return isSgForAllStatement(n);
}

template <>
SgUpcForAllStatement* is<SgUpcForAllStatement>(SgNode* n) {
  return isSgUpcForAllStatement(n);
}

template <>
SgCAFWithTeamStatement* is<SgCAFWithTeamStatement>(SgNode* n) {
  return isSgCAFWithTeamStatement(n);
}

template <>
SgJavaForEachStatement* is<SgJavaForEachStatement>(SgNode* n) {
  return isSgJavaForEachStatement(n);
}

template <>
SgJavaLabelStatement* is<SgJavaLabelStatement>(SgNode* n) {
  return isSgJavaLabelStatement(n);
}

template <>
SgMatlabForStatement* is<SgMatlabForStatement>(SgNode* n) {
  return isSgMatlabForStatement(n);
}

template <>
SgFunctionParameterScope* is<SgFunctionParameterScope>(SgNode* n) {
  return isSgFunctionParameterScope(n);
}

template <>
SgDeclarationScope* is<SgDeclarationScope>(SgNode* n) {
  return isSgDeclarationScope(n);
}

template <>
SgRangeBasedForStatement* is<SgRangeBasedForStatement>(SgNode* n) {
  return isSgRangeBasedForStatement(n);
}

template <>
SgJovialForThenStatement* is<SgJovialForThenStatement>(SgNode* n) {
  return isSgJovialForThenStatement(n);
}

template <>
SgAdaAcceptStmt* is<SgAdaAcceptStmt>(SgNode* n) {
  return isSgAdaAcceptStmt(n);
}

template <>
SgAdaPackageSpec* is<SgAdaPackageSpec>(SgNode* n) {
  return isSgAdaPackageSpec(n);
}

template <>
SgAdaPackageBody* is<SgAdaPackageBody>(SgNode* n) {
  return isSgAdaPackageBody(n);
}

template <>
SgAdaTaskSpec* is<SgAdaTaskSpec>(SgNode* n) {
  return isSgAdaTaskSpec(n);
}

template <>
SgAdaTaskBody* is<SgAdaTaskBody>(SgNode* n) {
  return isSgAdaTaskBody(n);
}

template <>
SgAdaProtectedBody* is<SgAdaProtectedBody>(SgNode* n) {
  return isSgAdaProtectedBody(n);
}

template <>
SgAdaProtectedSpec* is<SgAdaProtectedSpec>(SgNode* n) {
  return isSgAdaProtectedSpec(n);
}

template <>
SgAdaGenericDefn* is<SgAdaGenericDefn>(SgNode* n) {
  return isSgAdaGenericDefn(n);
}

template <>
SgFunctionTypeTable* is<SgFunctionTypeTable>(SgNode* n) {
  return isSgFunctionTypeTable(n);
}

template <>
SgDeclarationStatement* is<SgDeclarationStatement>(SgNode* n) {
  return isSgDeclarationStatement(n);
}

template <>
SgFunctionParameterList* is<SgFunctionParameterList>(SgNode* n) {
  return isSgFunctionParameterList(n);
}

template <>
SgVariableDeclaration* is<SgVariableDeclaration>(SgNode* n) {
  return isSgVariableDeclaration(n);
}

template <>
SgTemplateVariableDeclaration* is<SgTemplateVariableDeclaration>(SgNode* n) {
  return isSgTemplateVariableDeclaration(n);
}

template <>
SgVariableDefinition* is<SgVariableDefinition>(SgNode* n) {
  return isSgVariableDefinition(n);
}

template <>
SgClinkageDeclarationStatement* is<SgClinkageDeclarationStatement>(SgNode* n) {
  return isSgClinkageDeclarationStatement(n);
}

template <>
SgClinkageStartStatement* is<SgClinkageStartStatement>(SgNode* n) {
  return isSgClinkageStartStatement(n);
}

template <>
SgClinkageEndStatement* is<SgClinkageEndStatement>(SgNode* n) {
  return isSgClinkageEndStatement(n);
}

template <>
SgEnumDeclaration* is<SgEnumDeclaration>(SgNode* n) {
  return isSgEnumDeclaration(n);
}

template <>
SgAsmStmt* is<SgAsmStmt>(SgNode* n) {
  return isSgAsmStmt(n);
}

template <>
SgAttributeSpecificationStatement* is<SgAttributeSpecificationStatement>(
    SgNode* n) {
  return isSgAttributeSpecificationStatement(n);
}

template <>
SgFormatStatement* is<SgFormatStatement>(SgNode* n) {
  return isSgFormatStatement(n);
}

template <>
SgTemplateDeclaration* is<SgTemplateDeclaration>(SgNode* n) {
  return isSgTemplateDeclaration(n);
}

template <>
SgTemplateInstantiationDirectiveStatement*
is<SgTemplateInstantiationDirectiveStatement>(SgNode* n) {
  return isSgTemplateInstantiationDirectiveStatement(n);
}

template <>
SgUseStatement* is<SgUseStatement>(SgNode* n) {
  return isSgUseStatement(n);
}

template <>
SgParameterStatement* is<SgParameterStatement>(SgNode* n) {
  return isSgParameterStatement(n);
}

template <>
SgNamespaceDeclarationStatement* is<SgNamespaceDeclarationStatement>(
    SgNode* n) {
  return isSgNamespaceDeclarationStatement(n);
}

template <>
SgEquivalenceStatement* is<SgEquivalenceStatement>(SgNode* n) {
  return isSgEquivalenceStatement(n);
}

template <>
SgInterfaceStatement* is<SgInterfaceStatement>(SgNode* n) {
  return isSgInterfaceStatement(n);
}

template <>
SgNamespaceAliasDeclarationStatement* is<SgNamespaceAliasDeclarationStatement>(
    SgNode* n) {
  return isSgNamespaceAliasDeclarationStatement(n);
}

template <>
SgCommonBlock* is<SgCommonBlock>(SgNode* n) {
  return isSgCommonBlock(n);
}

template <>
SgTypedefDeclaration* is<SgTypedefDeclaration>(SgNode* n) {
  return isSgTypedefDeclaration(n);
}

template <>
SgTemplateTypedefDeclaration* is<SgTemplateTypedefDeclaration>(SgNode* n) {
  return isSgTemplateTypedefDeclaration(n);
}

template <>
SgTemplateInstantiationTypedefDeclaration*
is<SgTemplateInstantiationTypedefDeclaration>(SgNode* n) {
  return isSgTemplateInstantiationTypedefDeclaration(n);
}

template <>
SgStatementFunctionStatement* is<SgStatementFunctionStatement>(SgNode* n) {
  return isSgStatementFunctionStatement(n);
}

template <>
SgCtorInitializerList* is<SgCtorInitializerList>(SgNode* n) {
  return isSgCtorInitializerList(n);
}

template <>
SgPragmaDeclaration* is<SgPragmaDeclaration>(SgNode* n) {
  return isSgPragmaDeclaration(n);
}

template <>
SgUsingDirectiveStatement* is<SgUsingDirectiveStatement>(SgNode* n) {
  return isSgUsingDirectiveStatement(n);
}

template <>
SgClassDeclaration* is<SgClassDeclaration>(SgNode* n) {
  return isSgClassDeclaration(n);
}

template <>
SgTemplateClassDeclaration* is<SgTemplateClassDeclaration>(SgNode* n) {
  return isSgTemplateClassDeclaration(n);
}

template <>
SgTemplateInstantiationDecl* is<SgTemplateInstantiationDecl>(SgNode* n) {
  return isSgTemplateInstantiationDecl(n);
}

template <>
SgDerivedTypeStatement* is<SgDerivedTypeStatement>(SgNode* n) {
  return isSgDerivedTypeStatement(n);
}

template <>
SgModuleStatement* is<SgModuleStatement>(SgNode* n) {
  return isSgModuleStatement(n);
}

template <>
SgJavaPackageDeclaration* is<SgJavaPackageDeclaration>(SgNode* n) {
  return isSgJavaPackageDeclaration(n);
}

template <>
SgJovialTableStatement* is<SgJovialTableStatement>(SgNode* n) {
  return isSgJovialTableStatement(n);
}

template <>
SgImplicitStatement* is<SgImplicitStatement>(SgNode* n) {
  return isSgImplicitStatement(n);
}

template <>
SgUsingDeclarationStatement* is<SgUsingDeclarationStatement>(SgNode* n) {
  return isSgUsingDeclarationStatement(n);
}

template <>
SgNamelistStatement* is<SgNamelistStatement>(SgNode* n) {
  return isSgNamelistStatement(n);
}

template <>
SgImportStatement* is<SgImportStatement>(SgNode* n) {
  return isSgImportStatement(n);
}

template <>
SgFunctionDeclaration* is<SgFunctionDeclaration>(SgNode* n) {
  return isSgFunctionDeclaration(n);
}

template <>
SgTemplateFunctionDeclaration* is<SgTemplateFunctionDeclaration>(SgNode* n) {
  return isSgTemplateFunctionDeclaration(n);
}

template <>
SgMemberFunctionDeclaration* is<SgMemberFunctionDeclaration>(SgNode* n) {
  return isSgMemberFunctionDeclaration(n);
}

template <>
SgTemplateMemberFunctionDeclaration* is<SgTemplateMemberFunctionDeclaration>(
    SgNode* n) {
  return isSgTemplateMemberFunctionDeclaration(n);
}

template <>
SgTemplateInstantiationMemberFunctionDecl*
is<SgTemplateInstantiationMemberFunctionDecl>(SgNode* n) {
  return isSgTemplateInstantiationMemberFunctionDecl(n);
}

template <>
SgTemplateInstantiationFunctionDecl* is<SgTemplateInstantiationFunctionDecl>(
    SgNode* n) {
  return isSgTemplateInstantiationFunctionDecl(n);
}

template <>
SgProgramHeaderStatement* is<SgProgramHeaderStatement>(SgNode* n) {
  return isSgProgramHeaderStatement(n);
}

template <>
SgProcedureHeaderStatement* is<SgProcedureHeaderStatement>(SgNode* n) {
  return isSgProcedureHeaderStatement(n);
}

template <>
SgEntryStatement* is<SgEntryStatement>(SgNode* n) {
  return isSgEntryStatement(n);
}

template <>
SgAdaEntryDecl* is<SgAdaEntryDecl>(SgNode* n) {
  return isSgAdaEntryDecl(n);
}

template <>
SgAdaFunctionRenamingDecl* is<SgAdaFunctionRenamingDecl>(SgNode* n) {
  return isSgAdaFunctionRenamingDecl(n);
}

template <>
SgContainsStatement* is<SgContainsStatement>(SgNode* n) {
  return isSgContainsStatement(n);
}

template <>
SgC_PreprocessorDirectiveStatement* is<SgC_PreprocessorDirectiveStatement>(
    SgNode* n) {
  return isSgC_PreprocessorDirectiveStatement(n);
}

template <>
SgIncludeDirectiveStatement* is<SgIncludeDirectiveStatement>(SgNode* n) {
  return isSgIncludeDirectiveStatement(n);
}

template <>
SgDefineDirectiveStatement* is<SgDefineDirectiveStatement>(SgNode* n) {
  return isSgDefineDirectiveStatement(n);
}

template <>
SgUndefDirectiveStatement* is<SgUndefDirectiveStatement>(SgNode* n) {
  return isSgUndefDirectiveStatement(n);
}

template <>
SgIfdefDirectiveStatement* is<SgIfdefDirectiveStatement>(SgNode* n) {
  return isSgIfdefDirectiveStatement(n);
}

template <>
SgIfndefDirectiveStatement* is<SgIfndefDirectiveStatement>(SgNode* n) {
  return isSgIfndefDirectiveStatement(n);
}

template <>
SgIfDirectiveStatement* is<SgIfDirectiveStatement>(SgNode* n) {
  return isSgIfDirectiveStatement(n);
}

template <>
SgDeadIfDirectiveStatement* is<SgDeadIfDirectiveStatement>(SgNode* n) {
  return isSgDeadIfDirectiveStatement(n);
}

template <>
SgElseDirectiveStatement* is<SgElseDirectiveStatement>(SgNode* n) {
  return isSgElseDirectiveStatement(n);
}

template <>
SgElseifDirectiveStatement* is<SgElseifDirectiveStatement>(SgNode* n) {
  return isSgElseifDirectiveStatement(n);
}

template <>
SgEndifDirectiveStatement* is<SgEndifDirectiveStatement>(SgNode* n) {
  return isSgEndifDirectiveStatement(n);
}

template <>
SgLineDirectiveStatement* is<SgLineDirectiveStatement>(SgNode* n) {
  return isSgLineDirectiveStatement(n);
}

template <>
SgWarningDirectiveStatement* is<SgWarningDirectiveStatement>(SgNode* n) {
  return isSgWarningDirectiveStatement(n);
}

template <>
SgErrorDirectiveStatement* is<SgErrorDirectiveStatement>(SgNode* n) {
  return isSgErrorDirectiveStatement(n);
}

template <>
SgEmptyDirectiveStatement* is<SgEmptyDirectiveStatement>(SgNode* n) {
  return isSgEmptyDirectiveStatement(n);
}

template <>
SgIncludeNextDirectiveStatement* is<SgIncludeNextDirectiveStatement>(
    SgNode* n) {
  return isSgIncludeNextDirectiveStatement(n);
}

template <>
SgIdentDirectiveStatement* is<SgIdentDirectiveStatement>(SgNode* n) {
  return isSgIdentDirectiveStatement(n);
}

template <>
SgLinemarkerDirectiveStatement* is<SgLinemarkerDirectiveStatement>(SgNode* n) {
  return isSgLinemarkerDirectiveStatement(n);
}

template <>
SgOmpThreadprivateStatement* is<SgOmpThreadprivateStatement>(SgNode* n) {
  return isSgOmpThreadprivateStatement(n);
}

template <>
SgFortranIncludeLine* is<SgFortranIncludeLine>(SgNode* n) {
  return isSgFortranIncludeLine(n);
}

template <>
SgJavaImportStatement* is<SgJavaImportStatement>(SgNode* n) {
  return isSgJavaImportStatement(n);
}

template <>
SgJavaPackageStatement* is<SgJavaPackageStatement>(SgNode* n) {
  return isSgJavaPackageStatement(n);
}

template <>
SgStmtDeclarationStatement* is<SgStmtDeclarationStatement>(SgNode* n) {
  return isSgStmtDeclarationStatement(n);
}

template <>
SgStaticAssertionDeclaration* is<SgStaticAssertionDeclaration>(SgNode* n) {
  return isSgStaticAssertionDeclaration(n);
}

template <>
SgOmpDeclareSimdStatement* is<SgOmpDeclareSimdStatement>(SgNode* n) {
  return isSgOmpDeclareSimdStatement(n);
}

template <>
SgMicrosoftAttributeDeclaration* is<SgMicrosoftAttributeDeclaration>(
    SgNode* n) {
  return isSgMicrosoftAttributeDeclaration(n);
}

template <>
SgJovialCompoolStatement* is<SgJovialCompoolStatement>(SgNode* n) {
  return isSgJovialCompoolStatement(n);
}

template <>
SgJovialDirectiveStatement* is<SgJovialDirectiveStatement>(SgNode* n) {
  return isSgJovialDirectiveStatement(n);
}

template <>
SgJovialDefineDeclaration* is<SgJovialDefineDeclaration>(SgNode* n) {
  return isSgJovialDefineDeclaration(n);
}

template <>
SgJovialOverlayDeclaration* is<SgJovialOverlayDeclaration>(SgNode* n) {
  return isSgJovialOverlayDeclaration(n);
}

template <>
SgNonrealDecl* is<SgNonrealDecl>(SgNode* n) {
  return isSgNonrealDecl(n);
}

template <>
SgEmptyDeclaration* is<SgEmptyDeclaration>(SgNode* n) {
  return isSgEmptyDeclaration(n);
}

template <>
SgAdaPackageBodyDecl* is<SgAdaPackageBodyDecl>(SgNode* n) {
  return isSgAdaPackageBodyDecl(n);
}

template <>
SgAdaPackageSpecDecl* is<SgAdaPackageSpecDecl>(SgNode* n) {
  return isSgAdaPackageSpecDecl(n);
}

template <>
SgAdaRenamingDecl* is<SgAdaRenamingDecl>(SgNode* n) {
  return isSgAdaRenamingDecl(n);
}

template <>
SgAdaTaskSpecDecl* is<SgAdaTaskSpecDecl>(SgNode* n) {
  return isSgAdaTaskSpecDecl(n);
}

template <>
SgAdaTaskBodyDecl* is<SgAdaTaskBodyDecl>(SgNode* n) {
  return isSgAdaTaskBodyDecl(n);
}

template <>
SgAdaTaskTypeDecl* is<SgAdaTaskTypeDecl>(SgNode* n) {
  return isSgAdaTaskTypeDecl(n);
}

template <>
SgAdaProtectedSpecDecl* is<SgAdaProtectedSpecDecl>(SgNode* n) {
  return isSgAdaProtectedSpecDecl(n);
}

template <>
SgAdaProtectedBodyDecl* is<SgAdaProtectedBodyDecl>(SgNode* n) {
  return isSgAdaProtectedBodyDecl(n);
}

template <>
SgAdaProtectedTypeDecl* is<SgAdaProtectedTypeDecl>(SgNode* n) {
  return isSgAdaProtectedTypeDecl(n);
}

template <>
SgAdaRepresentationClause* is<SgAdaRepresentationClause>(SgNode* n) {
  return isSgAdaRepresentationClause(n);
}

template <>
SgAdaComponentClause* is<SgAdaComponentClause>(SgNode* n) {
  return isSgAdaComponentClause(n);
}

template <>
SgAdaAttributeClause* is<SgAdaAttributeClause>(SgNode* n) {
  return isSgAdaAttributeClause(n);
}

template <>
SgAdaEnumRepresentationClause* is<SgAdaEnumRepresentationClause>(SgNode* n) {
  return isSgAdaEnumRepresentationClause(n);
}

template <>
SgAdaGenericDecl* is<SgAdaGenericDecl>(SgNode* n) {
  return isSgAdaGenericDecl(n);
}

template <>
SgAdaFormalTypeDecl* is<SgAdaFormalTypeDecl>(SgNode* n) {
  return isSgAdaFormalTypeDecl(n);
}

template <>
SgAdaDiscriminatedTypeDecl* is<SgAdaDiscriminatedTypeDecl>(SgNode* n) {
  return isSgAdaDiscriminatedTypeDecl(n);
}

template <>
SgAdaGenericInstanceDecl* is<SgAdaGenericInstanceDecl>(SgNode* n) {
  return isSgAdaGenericInstanceDecl(n);
}

template <>
SgAdaFormalPackageDecl* is<SgAdaFormalPackageDecl>(SgNode* n) {
  return isSgAdaFormalPackageDecl(n);
}

template <>
SgAdaParameterList* is<SgAdaParameterList>(SgNode* n) {
  return isSgAdaParameterList(n);
}

template <>
SgAdaVariantDecl* is<SgAdaVariantDecl>(SgNode* n) {
  return isSgAdaVariantDecl(n);
}

template <>
SgExprStatement* is<SgExprStatement>(SgNode* n) {
  return isSgExprStatement(n);
}

template <>
SgLabelStatement* is<SgLabelStatement>(SgNode* n) {
  return isSgLabelStatement(n);
}

template <>
SgCaseOptionStmt* is<SgCaseOptionStmt>(SgNode* n) {
  return isSgCaseOptionStmt(n);
}

template <>
SgTryStmt* is<SgTryStmt>(SgNode* n) {
  return isSgTryStmt(n);
}

template <>
SgDefaultOptionStmt* is<SgDefaultOptionStmt>(SgNode* n) {
  return isSgDefaultOptionStmt(n);
}

template <>
SgBreakStmt* is<SgBreakStmt>(SgNode* n) {
  return isSgBreakStmt(n);
}

template <>
SgContinueStmt* is<SgContinueStmt>(SgNode* n) {
  return isSgContinueStmt(n);
}

template <>
SgReturnStmt* is<SgReturnStmt>(SgNode* n) {
  return isSgReturnStmt(n);
}

template <>
SgGotoStatement* is<SgGotoStatement>(SgNode* n) {
  return isSgGotoStatement(n);
}

template <>
SgSpawnStmt* is<SgSpawnStmt>(SgNode* n) {
  return isSgSpawnStmt(n);
}

template <>
SgNullStatement* is<SgNullStatement>(SgNode* n) {
  return isSgNullStatement(n);
}

template <>
SgVariantStatement* is<SgVariantStatement>(SgNode* n) {
  return isSgVariantStatement(n);
}

template <>
SgForInitStatement* is<SgForInitStatement>(SgNode* n) {
  return isSgForInitStatement(n);
}

template <>
SgCatchStatementSeq* is<SgCatchStatementSeq>(SgNode* n) {
  return isSgCatchStatementSeq(n);
}

template <>
SgProcessControlStatement* is<SgProcessControlStatement>(SgNode* n) {
  return isSgProcessControlStatement(n);
}

template <>
SgIOStatement* is<SgIOStatement>(SgNode* n) {
  return isSgIOStatement(n);
}

template <>
SgPrintStatement* is<SgPrintStatement>(SgNode* n) {
  return isSgPrintStatement(n);
}

template <>
SgReadStatement* is<SgReadStatement>(SgNode* n) {
  return isSgReadStatement(n);
}

template <>
SgWriteStatement* is<SgWriteStatement>(SgNode* n) {
  return isSgWriteStatement(n);
}

template <>
SgOpenStatement* is<SgOpenStatement>(SgNode* n) {
  return isSgOpenStatement(n);
}

template <>
SgCloseStatement* is<SgCloseStatement>(SgNode* n) {
  return isSgCloseStatement(n);
}

template <>
SgInquireStatement* is<SgInquireStatement>(SgNode* n) {
  return isSgInquireStatement(n);
}

template <>
SgFlushStatement* is<SgFlushStatement>(SgNode* n) {
  return isSgFlushStatement(n);
}

template <>
SgBackspaceStatement* is<SgBackspaceStatement>(SgNode* n) {
  return isSgBackspaceStatement(n);
}

template <>
SgRewindStatement* is<SgRewindStatement>(SgNode* n) {
  return isSgRewindStatement(n);
}

template <>
SgEndfileStatement* is<SgEndfileStatement>(SgNode* n) {
  return isSgEndfileStatement(n);
}

template <>
SgWaitStatement* is<SgWaitStatement>(SgNode* n) {
  return isSgWaitStatement(n);
}

template <>
SgWhereStatement* is<SgWhereStatement>(SgNode* n) {
  return isSgWhereStatement(n);
}

template <>
SgElseWhereStatement* is<SgElseWhereStatement>(SgNode* n) {
  return isSgElseWhereStatement(n);
}

template <>
SgNullifyStatement* is<SgNullifyStatement>(SgNode* n) {
  return isSgNullifyStatement(n);
}

template <>
SgArithmeticIfStatement* is<SgArithmeticIfStatement>(SgNode* n) {
  return isSgArithmeticIfStatement(n);
}

template <>
SgAssignStatement* is<SgAssignStatement>(SgNode* n) {
  return isSgAssignStatement(n);
}

template <>
SgComputedGotoStatement* is<SgComputedGotoStatement>(SgNode* n) {
  return isSgComputedGotoStatement(n);
}

template <>
SgAssignedGotoStatement* is<SgAssignedGotoStatement>(SgNode* n) {
  return isSgAssignedGotoStatement(n);
}

template <>
SgAllocateStatement* is<SgAllocateStatement>(SgNode* n) {
  return isSgAllocateStatement(n);
}

template <>
SgDeallocateStatement* is<SgDeallocateStatement>(SgNode* n) {
  return isSgDeallocateStatement(n);
}

template <>
SgUpcNotifyStatement* is<SgUpcNotifyStatement>(SgNode* n) {
  return isSgUpcNotifyStatement(n);
}

template <>
SgUpcWaitStatement* is<SgUpcWaitStatement>(SgNode* n) {
  return isSgUpcWaitStatement(n);
}

template <>
SgUpcBarrierStatement* is<SgUpcBarrierStatement>(SgNode* n) {
  return isSgUpcBarrierStatement(n);
}

template <>
SgUpcFenceStatement* is<SgUpcFenceStatement>(SgNode* n) {
  return isSgUpcFenceStatement(n);
}

template <>
SgOmpBarrierStatement* is<SgOmpBarrierStatement>(SgNode* n) {
  return isSgOmpBarrierStatement(n);
}

template <>
SgOmpTaskwaitStatement* is<SgOmpTaskwaitStatement>(SgNode* n) {
  return isSgOmpTaskwaitStatement(n);
}

template <>
SgOmpFlushStatement* is<SgOmpFlushStatement>(SgNode* n) {
  return isSgOmpFlushStatement(n);
}

template <>
SgOmpBodyStatement* is<SgOmpBodyStatement>(SgNode* n) {
  return isSgOmpBodyStatement(n);
}

template <>
SgOmpMasterStatement* is<SgOmpMasterStatement>(SgNode* n) {
  return isSgOmpMasterStatement(n);
}

template <>
SgOmpOrderedStatement* is<SgOmpOrderedStatement>(SgNode* n) {
  return isSgOmpOrderedStatement(n);
}

template <>
SgOmpCriticalStatement* is<SgOmpCriticalStatement>(SgNode* n) {
  return isSgOmpCriticalStatement(n);
}

template <>
SgOmpSectionStatement* is<SgOmpSectionStatement>(SgNode* n) {
  return isSgOmpSectionStatement(n);
}

template <>
SgOmpWorkshareStatement* is<SgOmpWorkshareStatement>(SgNode* n) {
  return isSgOmpWorkshareStatement(n);
}

template <>
SgOmpClauseBodyStatement* is<SgOmpClauseBodyStatement>(SgNode* n) {
  return isSgOmpClauseBodyStatement(n);
}

template <>
SgOmpParallelStatement* is<SgOmpParallelStatement>(SgNode* n) {
  return isSgOmpParallelStatement(n);
}

template <>
SgOmpSingleStatement* is<SgOmpSingleStatement>(SgNode* n) {
  return isSgOmpSingleStatement(n);
}

template <>
SgOmpAtomicStatement* is<SgOmpAtomicStatement>(SgNode* n) {
  return isSgOmpAtomicStatement(n);
}

template <>
SgOmpTaskStatement* is<SgOmpTaskStatement>(SgNode* n) {
  return isSgOmpTaskStatement(n);
}

template <>
SgOmpForStatement* is<SgOmpForStatement>(SgNode* n) {
  return isSgOmpForStatement(n);
}

template <>
SgOmpDoStatement* is<SgOmpDoStatement>(SgNode* n) {
  return isSgOmpDoStatement(n);
}

template <>
SgOmpSectionsStatement* is<SgOmpSectionsStatement>(SgNode* n) {
  return isSgOmpSectionsStatement(n);
}

template <>
SgOmpTargetStatement* is<SgOmpTargetStatement>(SgNode* n) {
  return isSgOmpTargetStatement(n);
}

template <>
SgOmpTargetDataStatement* is<SgOmpTargetDataStatement>(SgNode* n) {
  return isSgOmpTargetDataStatement(n);
}

template <>
SgOmpSimdStatement* is<SgOmpSimdStatement>(SgNode* n) {
  return isSgOmpSimdStatement(n);
}

template <>
SgOmpForSimdStatement* is<SgOmpForSimdStatement>(SgNode* n) {
  return isSgOmpForSimdStatement(n);
}

template <>
SgSequenceStatement* is<SgSequenceStatement>(SgNode* n) {
  return isSgSequenceStatement(n);
}

template <>
SgWithStatement* is<SgWithStatement>(SgNode* n) {
  return isSgWithStatement(n);
}

template <>
SgPythonPrintStmt* is<SgPythonPrintStmt>(SgNode* n) {
  return isSgPythonPrintStmt(n);
}

template <>
SgPassStatement* is<SgPassStatement>(SgNode* n) {
  return isSgPassStatement(n);
}

template <>
SgAssertStmt* is<SgAssertStmt>(SgNode* n) {
  return isSgAssertStmt(n);
}

template <>
SgExecStatement* is<SgExecStatement>(SgNode* n) {
  return isSgExecStatement(n);
}

template <>
SgPythonGlobalStmt* is<SgPythonGlobalStmt>(SgNode* n) {
  return isSgPythonGlobalStmt(n);
}

template <>
SgJavaThrowStatement* is<SgJavaThrowStatement>(SgNode* n) {
  return isSgJavaThrowStatement(n);
}

template <>
SgJavaSynchronizedStatement* is<SgJavaSynchronizedStatement>(SgNode* n) {
  return isSgJavaSynchronizedStatement(n);
}

template <>
SgAsyncStmt* is<SgAsyncStmt>(SgNode* n) {
  return isSgAsyncStmt(n);
}

template <>
SgFinishStmt* is<SgFinishStmt>(SgNode* n) {
  return isSgFinishStmt(n);
}

template <>
SgAtStmt* is<SgAtStmt>(SgNode* n) {
  return isSgAtStmt(n);
}

template <>
SgAtomicStmt* is<SgAtomicStmt>(SgNode* n) {
  return isSgAtomicStmt(n);
}

template <>
SgWhenStmt* is<SgWhenStmt>(SgNode* n) {
  return isSgWhenStmt(n);
}

template <>
SgImageControlStatement* is<SgImageControlStatement>(SgNode* n) {
  return isSgImageControlStatement(n);
}

template <>
SgSyncAllStatement* is<SgSyncAllStatement>(SgNode* n) {
  return isSgSyncAllStatement(n);
}

template <>
SgSyncImagesStatement* is<SgSyncImagesStatement>(SgNode* n) {
  return isSgSyncImagesStatement(n);
}

template <>
SgSyncMemoryStatement* is<SgSyncMemoryStatement>(SgNode* n) {
  return isSgSyncMemoryStatement(n);
}

template <>
SgSyncTeamStatement* is<SgSyncTeamStatement>(SgNode* n) {
  return isSgSyncTeamStatement(n);
}

template <>
SgLockStatement* is<SgLockStatement>(SgNode* n) {
  return isSgLockStatement(n);
}

template <>
SgUnlockStatement* is<SgUnlockStatement>(SgNode* n) {
  return isSgUnlockStatement(n);
}

template <>
SgAdaExitStmt* is<SgAdaExitStmt>(SgNode* n) {
  return isSgAdaExitStmt(n);
}

template <>
SgAdaDelayStmt* is<SgAdaDelayStmt>(SgNode* n) {
  return isSgAdaDelayStmt(n);
}

template <>
SgAdaLoopStmt* is<SgAdaLoopStmt>(SgNode* n) {
  return isSgAdaLoopStmt(n);
}

template <>
SgAdaSelectStmt* is<SgAdaSelectStmt>(SgNode* n) {
  return isSgAdaSelectStmt(n);
}

template <>
SgAdaSelectAlternativeStmt* is<SgAdaSelectAlternativeStmt>(SgNode* n) {
  return isSgAdaSelectAlternativeStmt(n);
}

template <>
SgAdaTerminateStmt* is<SgAdaTerminateStmt>(SgNode* n) {
  return isSgAdaTerminateStmt(n);
}

template <>
SgAdaUnscopedBlock* is<SgAdaUnscopedBlock>(SgNode* n) {
  return isSgAdaUnscopedBlock(n);
}

template <>
SgAdaVariantWhenStmt* is<SgAdaVariantWhenStmt>(SgNode* n) {
  return isSgAdaVariantWhenStmt(n);
}

template <>
SgExpression* is<SgExpression>(SgNode* n) {
  return isSgExpression(n);
}

template <>
SgUnaryOp* is<SgUnaryOp>(SgNode* n) {
  return isSgUnaryOp(n);
}

template <>
SgExpressionRoot* is<SgExpressionRoot>(SgNode* n) {
  return isSgExpressionRoot(n);
}

template <>
SgMinusOp* is<SgMinusOp>(SgNode* n) {
  return isSgMinusOp(n);
}

template <>
SgUnaryAddOp* is<SgUnaryAddOp>(SgNode* n) {
  return isSgUnaryAddOp(n);
}

template <>
SgNotOp* is<SgNotOp>(SgNode* n) {
  return isSgNotOp(n);
}

template <>
SgPointerDerefExp* is<SgPointerDerefExp>(SgNode* n) {
  return isSgPointerDerefExp(n);
}

template <>
SgAddressOfOp* is<SgAddressOfOp>(SgNode* n) {
  return isSgAddressOfOp(n);
}

template <>
SgMinusMinusOp* is<SgMinusMinusOp>(SgNode* n) {
  return isSgMinusMinusOp(n);
}

template <>
SgPlusPlusOp* is<SgPlusPlusOp>(SgNode* n) {
  return isSgPlusPlusOp(n);
}

template <>
SgBitComplementOp* is<SgBitComplementOp>(SgNode* n) {
  return isSgBitComplementOp(n);
}

template <>
SgCastExp* is<SgCastExp>(SgNode* n) {
  return isSgCastExp(n);
}

template <>
SgThrowOp* is<SgThrowOp>(SgNode* n) {
  return isSgThrowOp(n);
}

template <>
SgRealPartOp* is<SgRealPartOp>(SgNode* n) {
  return isSgRealPartOp(n);
}

template <>
SgImagPartOp* is<SgImagPartOp>(SgNode* n) {
  return isSgImagPartOp(n);
}

template <>
SgConjugateOp* is<SgConjugateOp>(SgNode* n) {
  return isSgConjugateOp(n);
}

template <>
SgUserDefinedUnaryOp* is<SgUserDefinedUnaryOp>(SgNode* n) {
  return isSgUserDefinedUnaryOp(n);
}

template <>
SgMatrixTransposeOp* is<SgMatrixTransposeOp>(SgNode* n) {
  return isSgMatrixTransposeOp(n);
}

template <>
SgAbsOp* is<SgAbsOp>(SgNode* n) {
  return isSgAbsOp(n);
}

template <>
SgBinaryOp* is<SgBinaryOp>(SgNode* n) {
  return isSgBinaryOp(n);
}

template <>
SgArrowExp* is<SgArrowExp>(SgNode* n) {
  return isSgArrowExp(n);
}

template <>
SgDotExp* is<SgDotExp>(SgNode* n) {
  return isSgDotExp(n);
}

template <>
SgDotStarOp* is<SgDotStarOp>(SgNode* n) {
  return isSgDotStarOp(n);
}

template <>
SgArrowStarOp* is<SgArrowStarOp>(SgNode* n) {
  return isSgArrowStarOp(n);
}

template <>
SgEqualityOp* is<SgEqualityOp>(SgNode* n) {
  return isSgEqualityOp(n);
}

template <>
SgLessThanOp* is<SgLessThanOp>(SgNode* n) {
  return isSgLessThanOp(n);
}

template <>
SgGreaterThanOp* is<SgGreaterThanOp>(SgNode* n) {
  return isSgGreaterThanOp(n);
}

template <>
SgNotEqualOp* is<SgNotEqualOp>(SgNode* n) {
  return isSgNotEqualOp(n);
}

template <>
SgLessOrEqualOp* is<SgLessOrEqualOp>(SgNode* n) {
  return isSgLessOrEqualOp(n);
}

template <>
SgGreaterOrEqualOp* is<SgGreaterOrEqualOp>(SgNode* n) {
  return isSgGreaterOrEqualOp(n);
}

template <>
SgAddOp* is<SgAddOp>(SgNode* n) {
  return isSgAddOp(n);
}

template <>
SgSubtractOp* is<SgSubtractOp>(SgNode* n) {
  return isSgSubtractOp(n);
}

template <>
SgMultiplyOp* is<SgMultiplyOp>(SgNode* n) {
  return isSgMultiplyOp(n);
}

template <>
SgDivideOp* is<SgDivideOp>(SgNode* n) {
  return isSgDivideOp(n);
}

template <>
SgIntegerDivideOp* is<SgIntegerDivideOp>(SgNode* n) {
  return isSgIntegerDivideOp(n);
}

template <>
SgModOp* is<SgModOp>(SgNode* n) {
  return isSgModOp(n);
}

template <>
SgAndOp* is<SgAndOp>(SgNode* n) {
  return isSgAndOp(n);
}

template <>
SgOrOp* is<SgOrOp>(SgNode* n) {
  return isSgOrOp(n);
}

template <>
SgBitXorOp* is<SgBitXorOp>(SgNode* n) {
  return isSgBitXorOp(n);
}

template <>
SgBitAndOp* is<SgBitAndOp>(SgNode* n) {
  return isSgBitAndOp(n);
}

template <>
SgBitOrOp* is<SgBitOrOp>(SgNode* n) {
  return isSgBitOrOp(n);
}

template <>
SgBitEqvOp* is<SgBitEqvOp>(SgNode* n) {
  return isSgBitEqvOp(n);
}

template <>
SgCommaOpExp* is<SgCommaOpExp>(SgNode* n) {
  return isSgCommaOpExp(n);
}

template <>
SgLshiftOp* is<SgLshiftOp>(SgNode* n) {
  return isSgLshiftOp(n);
}

template <>
SgRshiftOp* is<SgRshiftOp>(SgNode* n) {
  return isSgRshiftOp(n);
}

template <>
SgPntrArrRefExp* is<SgPntrArrRefExp>(SgNode* n) {
  return isSgPntrArrRefExp(n);
}

template <>
SgScopeOp* is<SgScopeOp>(SgNode* n) {
  return isSgScopeOp(n);
}

template <>
SgAssignOp* is<SgAssignOp>(SgNode* n) {
  return isSgAssignOp(n);
}

template <>
SgExponentiationOp* is<SgExponentiationOp>(SgNode* n) {
  return isSgExponentiationOp(n);
}

template <>
SgJavaUnsignedRshiftOp* is<SgJavaUnsignedRshiftOp>(SgNode* n) {
  return isSgJavaUnsignedRshiftOp(n);
}

template <>
SgConcatenationOp* is<SgConcatenationOp>(SgNode* n) {
  return isSgConcatenationOp(n);
}

template <>
SgPointerAssignOp* is<SgPointerAssignOp>(SgNode* n) {
  return isSgPointerAssignOp(n);
}

template <>
SgUserDefinedBinaryOp* is<SgUserDefinedBinaryOp>(SgNode* n) {
  return isSgUserDefinedBinaryOp(n);
}

template <>
SgCompoundAssignOp* is<SgCompoundAssignOp>(SgNode* n) {
  return isSgCompoundAssignOp(n);
}

template <>
SgPlusAssignOp* is<SgPlusAssignOp>(SgNode* n) {
  return isSgPlusAssignOp(n);
}

template <>
SgMinusAssignOp* is<SgMinusAssignOp>(SgNode* n) {
  return isSgMinusAssignOp(n);
}

template <>
SgAndAssignOp* is<SgAndAssignOp>(SgNode* n) {
  return isSgAndAssignOp(n);
}

template <>
SgIorAssignOp* is<SgIorAssignOp>(SgNode* n) {
  return isSgIorAssignOp(n);
}

template <>
SgMultAssignOp* is<SgMultAssignOp>(SgNode* n) {
  return isSgMultAssignOp(n);
}

template <>
SgDivAssignOp* is<SgDivAssignOp>(SgNode* n) {
  return isSgDivAssignOp(n);
}

template <>
SgModAssignOp* is<SgModAssignOp>(SgNode* n) {
  return isSgModAssignOp(n);
}

template <>
SgXorAssignOp* is<SgXorAssignOp>(SgNode* n) {
  return isSgXorAssignOp(n);
}

template <>
SgLshiftAssignOp* is<SgLshiftAssignOp>(SgNode* n) {
  return isSgLshiftAssignOp(n);
}

template <>
SgRshiftAssignOp* is<SgRshiftAssignOp>(SgNode* n) {
  return isSgRshiftAssignOp(n);
}

template <>
SgJavaUnsignedRshiftAssignOp* is<SgJavaUnsignedRshiftAssignOp>(SgNode* n) {
  return isSgJavaUnsignedRshiftAssignOp(n);
}

template <>
SgIntegerDivideAssignOp* is<SgIntegerDivideAssignOp>(SgNode* n) {
  return isSgIntegerDivideAssignOp(n);
}

template <>
SgExponentiationAssignOp* is<SgExponentiationAssignOp>(SgNode* n) {
  return isSgExponentiationAssignOp(n);
}

template <>
SgMembershipOp* is<SgMembershipOp>(SgNode* n) {
  return isSgMembershipOp(n);
}

template <>
SgSpaceshipOp* is<SgSpaceshipOp>(SgNode* n) {
  return isSgSpaceshipOp(n);
}

template <>
SgNonMembershipOp* is<SgNonMembershipOp>(SgNode* n) {
  return isSgNonMembershipOp(n);
}

template <>
SgIsOp* is<SgIsOp>(SgNode* n) {
  return isSgIsOp(n);
}

template <>
SgIsNotOp* is<SgIsNotOp>(SgNode* n) {
  return isSgIsNotOp(n);
}

template <>
SgDotDotExp* is<SgDotDotExp>(SgNode* n) {
  return isSgDotDotExp(n);
}

template <>
SgElementwiseOp* is<SgElementwiseOp>(SgNode* n) {
  return isSgElementwiseOp(n);
}

template <>
SgElementwiseMultiplyOp* is<SgElementwiseMultiplyOp>(SgNode* n) {
  return isSgElementwiseMultiplyOp(n);
}

template <>
SgElementwisePowerOp* is<SgElementwisePowerOp>(SgNode* n) {
  return isSgElementwisePowerOp(n);
}

template <>
SgElementwiseLeftDivideOp* is<SgElementwiseLeftDivideOp>(SgNode* n) {
  return isSgElementwiseLeftDivideOp(n);
}

template <>
SgElementwiseDivideOp* is<SgElementwiseDivideOp>(SgNode* n) {
  return isSgElementwiseDivideOp(n);
}

template <>
SgElementwiseAddOp* is<SgElementwiseAddOp>(SgNode* n) {
  return isSgElementwiseAddOp(n);
}

template <>
SgElementwiseSubtractOp* is<SgElementwiseSubtractOp>(SgNode* n) {
  return isSgElementwiseSubtractOp(n);
}

template <>
SgPowerOp* is<SgPowerOp>(SgNode* n) {
  return isSgPowerOp(n);
}

template <>
SgLeftDivideOp* is<SgLeftDivideOp>(SgNode* n) {
  return isSgLeftDivideOp(n);
}

template <>
SgRemOp* is<SgRemOp>(SgNode* n) {
  return isSgRemOp(n);
}

template <>
SgReplicationOp* is<SgReplicationOp>(SgNode* n) {
  return isSgReplicationOp(n);
}

template <>
SgAtOp* is<SgAtOp>(SgNode* n) {
  return isSgAtOp(n);
}

template <>
SgExprListExp* is<SgExprListExp>(SgNode* n) {
  return isSgExprListExp(n);
}

template <>
SgListExp* is<SgListExp>(SgNode* n) {
  return isSgListExp(n);
}

template <>
SgTupleExp* is<SgTupleExp>(SgNode* n) {
  return isSgTupleExp(n);
}

template <>
SgMatrixExp* is<SgMatrixExp>(SgNode* n) {
  return isSgMatrixExp(n);
}

template <>
SgVarRefExp* is<SgVarRefExp>(SgNode* n) {
  return isSgVarRefExp(n);
}

template <>
SgClassNameRefExp* is<SgClassNameRefExp>(SgNode* n) {
  return isSgClassNameRefExp(n);
}

template <>
SgFunctionRefExp* is<SgFunctionRefExp>(SgNode* n) {
  return isSgFunctionRefExp(n);
}

template <>
SgMemberFunctionRefExp* is<SgMemberFunctionRefExp>(SgNode* n) {
  return isSgMemberFunctionRefExp(n);
}

template <>
SgValueExp* is<SgValueExp>(SgNode* n) {
  return isSgValueExp(n);
}

template <>
SgBoolValExp* is<SgBoolValExp>(SgNode* n) {
  return isSgBoolValExp(n);
}

template <>
SgStringVal* is<SgStringVal>(SgNode* n) {
  return isSgStringVal(n);
}

template <>
SgShortVal* is<SgShortVal>(SgNode* n) {
  return isSgShortVal(n);
}

template <>
SgCharVal* is<SgCharVal>(SgNode* n) {
  return isSgCharVal(n);
}

template <>
SgUnsignedCharVal* is<SgUnsignedCharVal>(SgNode* n) {
  return isSgUnsignedCharVal(n);
}

template <>
SgWcharVal* is<SgWcharVal>(SgNode* n) {
  return isSgWcharVal(n);
}

template <>
SgUnsignedShortVal* is<SgUnsignedShortVal>(SgNode* n) {
  return isSgUnsignedShortVal(n);
}

template <>
SgIntVal* is<SgIntVal>(SgNode* n) {
  return isSgIntVal(n);
}

template <>
SgEnumVal* is<SgEnumVal>(SgNode* n) {
  return isSgEnumVal(n);
}

template <>
SgUnsignedIntVal* is<SgUnsignedIntVal>(SgNode* n) {
  return isSgUnsignedIntVal(n);
}

template <>
SgLongIntVal* is<SgLongIntVal>(SgNode* n) {
  return isSgLongIntVal(n);
}

template <>
SgLongLongIntVal* is<SgLongLongIntVal>(SgNode* n) {
  return isSgLongLongIntVal(n);
}

template <>
SgUnsignedLongLongIntVal* is<SgUnsignedLongLongIntVal>(SgNode* n) {
  return isSgUnsignedLongLongIntVal(n);
}

template <>
SgUnsignedLongVal* is<SgUnsignedLongVal>(SgNode* n) {
  return isSgUnsignedLongVal(n);
}

template <>
SgFloatVal* is<SgFloatVal>(SgNode* n) {
  return isSgFloatVal(n);
}

template <>
SgDoubleVal* is<SgDoubleVal>(SgNode* n) {
  return isSgDoubleVal(n);
}

template <>
SgLongDoubleVal* is<SgLongDoubleVal>(SgNode* n) {
  return isSgLongDoubleVal(n);
}

template <>
SgComplexVal* is<SgComplexVal>(SgNode* n) {
  return isSgComplexVal(n);
}

template <>
SgUpcThreads* is<SgUpcThreads>(SgNode* n) {
  return isSgUpcThreads(n);
}

template <>
SgUpcMythread* is<SgUpcMythread>(SgNode* n) {
  return isSgUpcMythread(n);
}

template <>
SgTemplateParameterVal* is<SgTemplateParameterVal>(SgNode* n) {
  return isSgTemplateParameterVal(n);
}

template <>
SgNullptrValExp* is<SgNullptrValExp>(SgNode* n) {
  return isSgNullptrValExp(n);
}

template <>
SgChar16Val* is<SgChar16Val>(SgNode* n) {
  return isSgChar16Val(n);
}

template <>
SgChar32Val* is<SgChar32Val>(SgNode* n) {
  return isSgChar32Val(n);
}

template <>
SgFloat80Val* is<SgFloat80Val>(SgNode* n) {
  return isSgFloat80Val(n);
}

template <>
SgFloat128Val* is<SgFloat128Val>(SgNode* n) {
  return isSgFloat128Val(n);
}

template <>
SgVoidVal* is<SgVoidVal>(SgNode* n) {
  return isSgVoidVal(n);
}

template <>
SgAdaFloatVal* is<SgAdaFloatVal>(SgNode* n) {
  return isSgAdaFloatVal(n);
}

template <>
SgJovialBitVal* is<SgJovialBitVal>(SgNode* n) {
  return isSgJovialBitVal(n);
}

template <>
SgCallExpression* is<SgCallExpression>(SgNode* n) {
  return isSgCallExpression(n);
}

template <>
SgFunctionCallExp* is<SgFunctionCallExp>(SgNode* n) {
  return isSgFunctionCallExp(n);
}

template <>
SgCudaKernelCallExp* is<SgCudaKernelCallExp>(SgNode* n) {
  return isSgCudaKernelCallExp(n);
}

template <>
SgSizeOfOp* is<SgSizeOfOp>(SgNode* n) {
  return isSgSizeOfOp(n);
}

template <>
SgUpcLocalsizeofExpression* is<SgUpcLocalsizeofExpression>(SgNode* n) {
  return isSgUpcLocalsizeofExpression(n);
}

template <>
SgUpcBlocksizeofExpression* is<SgUpcBlocksizeofExpression>(SgNode* n) {
  return isSgUpcBlocksizeofExpression(n);
}

template <>
SgUpcElemsizeofExpression* is<SgUpcElemsizeofExpression>(SgNode* n) {
  return isSgUpcElemsizeofExpression(n);
}

template <>
SgJavaInstanceOfOp* is<SgJavaInstanceOfOp>(SgNode* n) {
  return isSgJavaInstanceOfOp(n);
}

template <>
SgSuperExp* is<SgSuperExp>(SgNode* n) {
  return isSgSuperExp(n);
}

template <>
SgTypeIdOp* is<SgTypeIdOp>(SgNode* n) {
  return isSgTypeIdOp(n);
}

template <>
SgConditionalExp* is<SgConditionalExp>(SgNode* n) {
  return isSgConditionalExp(n);
}

template <>
SgNewExp* is<SgNewExp>(SgNode* n) {
  return isSgNewExp(n);
}

template <>
SgDeleteExp* is<SgDeleteExp>(SgNode* n) {
  return isSgDeleteExp(n);
}

template <>
SgThisExp* is<SgThisExp>(SgNode* n) {
  return isSgThisExp(n);
}

template <>
SgRefExp* is<SgRefExp>(SgNode* n) {
  return isSgRefExp(n);
}

template <>
SgInitializer* is<SgInitializer>(SgNode* n) {
  return isSgInitializer(n);
}

template <>
SgAggregateInitializer* is<SgAggregateInitializer>(SgNode* n) {
  return isSgAggregateInitializer(n);
}

template <>
SgCompoundInitializer* is<SgCompoundInitializer>(SgNode* n) {
  return isSgCompoundInitializer(n);
}

template <>
SgConstructorInitializer* is<SgConstructorInitializer>(SgNode* n) {
  return isSgConstructorInitializer(n);
}

template <>
SgAssignInitializer* is<SgAssignInitializer>(SgNode* n) {
  return isSgAssignInitializer(n);
}

template <>
SgDesignatedInitializer* is<SgDesignatedInitializer>(SgNode* n) {
  return isSgDesignatedInitializer(n);
}

template <>
SgBracedInitializer* is<SgBracedInitializer>(SgNode* n) {
  return isSgBracedInitializer(n);
}

template <>
SgAdaAncestorInitializer* is<SgAdaAncestorInitializer>(SgNode* n) {
  return isSgAdaAncestorInitializer(n);
}

template <>
SgVarArgStartOp* is<SgVarArgStartOp>(SgNode* n) {
  return isSgVarArgStartOp(n);
}

template <>
SgVarArgOp* is<SgVarArgOp>(SgNode* n) {
  return isSgVarArgOp(n);
}

template <>
SgVarArgEndOp* is<SgVarArgEndOp>(SgNode* n) {
  return isSgVarArgEndOp(n);
}

template <>
SgVarArgCopyOp* is<SgVarArgCopyOp>(SgNode* n) {
  return isSgVarArgCopyOp(n);
}

template <>
SgVarArgStartOneOperandOp* is<SgVarArgStartOneOperandOp>(SgNode* n) {
  return isSgVarArgStartOneOperandOp(n);
}

template <>
SgNullExpression* is<SgNullExpression>(SgNode* n) {
  return isSgNullExpression(n);
}

template <>
SgVariantExpression* is<SgVariantExpression>(SgNode* n) {
  return isSgVariantExpression(n);
}

template <>
SgSubscriptExpression* is<SgSubscriptExpression>(SgNode* n) {
  return isSgSubscriptExpression(n);
}

template <>
SgColonShapeExp* is<SgColonShapeExp>(SgNode* n) {
  return isSgColonShapeExp(n);
}

template <>
SgAsteriskShapeExp* is<SgAsteriskShapeExp>(SgNode* n) {
  return isSgAsteriskShapeExp(n);
}

template <>
SgImpliedDo* is<SgImpliedDo>(SgNode* n) {
  return isSgImpliedDo(n);
}

template <>
SgIOItemExpression* is<SgIOItemExpression>(SgNode* n) {
  return isSgIOItemExpression(n);
}

template <>
SgStatementExpression* is<SgStatementExpression>(SgNode* n) {
  return isSgStatementExpression(n);
}

template <>
SgAsmOp* is<SgAsmOp>(SgNode* n) {
  return isSgAsmOp(n);
}

template <>
SgLabelRefExp* is<SgLabelRefExp>(SgNode* n) {
  return isSgLabelRefExp(n);
}

template <>
SgActualArgumentExpression* is<SgActualArgumentExpression>(SgNode* n) {
  return isSgActualArgumentExpression(n);
}

template <>
SgUnknownArrayOrFunctionReference* is<SgUnknownArrayOrFunctionReference>(
    SgNode* n) {
  return isSgUnknownArrayOrFunctionReference(n);
}

template <>
SgPseudoDestructorRefExp* is<SgPseudoDestructorRefExp>(SgNode* n) {
  return isSgPseudoDestructorRefExp(n);
}

template <>
SgCAFCoExpression* is<SgCAFCoExpression>(SgNode* n) {
  return isSgCAFCoExpression(n);
}

template <>
SgCudaKernelExecConfig* is<SgCudaKernelExecConfig>(SgNode* n) {
  return isSgCudaKernelExecConfig(n);
}

template <>
SgLambdaRefExp* is<SgLambdaRefExp>(SgNode* n) {
  return isSgLambdaRefExp(n);
}

template <>
SgDictionaryExp* is<SgDictionaryExp>(SgNode* n) {
  return isSgDictionaryExp(n);
}

template <>
SgKeyDatumPair* is<SgKeyDatumPair>(SgNode* n) {
  return isSgKeyDatumPair(n);
}

template <>
SgComprehension* is<SgComprehension>(SgNode* n) {
  return isSgComprehension(n);
}

template <>
SgListComprehension* is<SgListComprehension>(SgNode* n) {
  return isSgListComprehension(n);
}

template <>
SgSetComprehension* is<SgSetComprehension>(SgNode* n) {
  return isSgSetComprehension(n);
}

template <>
SgDictionaryComprehension* is<SgDictionaryComprehension>(SgNode* n) {
  return isSgDictionaryComprehension(n);
}

template <>
SgNaryOp* is<SgNaryOp>(SgNode* n) {
  return isSgNaryOp(n);
}

template <>
SgNaryBooleanOp* is<SgNaryBooleanOp>(SgNode* n) {
  return isSgNaryBooleanOp(n);
}

template <>
SgNaryComparisonOp* is<SgNaryComparisonOp>(SgNode* n) {
  return isSgNaryComparisonOp(n);
}

template <>
SgStringConversion* is<SgStringConversion>(SgNode* n) {
  return isSgStringConversion(n);
}

template <>
SgYieldExpression* is<SgYieldExpression>(SgNode* n) {
  return isSgYieldExpression(n);
}

template <>
SgTemplateFunctionRefExp* is<SgTemplateFunctionRefExp>(SgNode* n) {
  return isSgTemplateFunctionRefExp(n);
}

template <>
SgTemplateMemberFunctionRefExp* is<SgTemplateMemberFunctionRefExp>(SgNode* n) {
  return isSgTemplateMemberFunctionRefExp(n);
}

template <>
SgAlignOfOp* is<SgAlignOfOp>(SgNode* n) {
  return isSgAlignOfOp(n);
}

template <>
SgRangeExp* is<SgRangeExp>(SgNode* n) {
  return isSgRangeExp(n);
}

template <>
SgMagicColonExp* is<SgMagicColonExp>(SgNode* n) {
  return isSgMagicColonExp(n);
}

template <>
SgTypeTraitBuiltinOperator* is<SgTypeTraitBuiltinOperator>(SgNode* n) {
  return isSgTypeTraitBuiltinOperator(n);
}

template <>
SgCompoundLiteralExp* is<SgCompoundLiteralExp>(SgNode* n) {
  return isSgCompoundLiteralExp(n);
}

template <>
SgJavaAnnotation* is<SgJavaAnnotation>(SgNode* n) {
  return isSgJavaAnnotation(n);
}

template <>
SgJavaMarkerAnnotation* is<SgJavaMarkerAnnotation>(SgNode* n) {
  return isSgJavaMarkerAnnotation(n);
}

template <>
SgJavaSingleMemberAnnotation* is<SgJavaSingleMemberAnnotation>(SgNode* n) {
  return isSgJavaSingleMemberAnnotation(n);
}

template <>
SgJavaNormalAnnotation* is<SgJavaNormalAnnotation>(SgNode* n) {
  return isSgJavaNormalAnnotation(n);
}

template <>
SgJavaTypeExpression* is<SgJavaTypeExpression>(SgNode* n) {
  return isSgJavaTypeExpression(n);
}

template <>
SgTypeExpression* is<SgTypeExpression>(SgNode* n) {
  return isSgTypeExpression(n);
}

template <>
SgClassExp* is<SgClassExp>(SgNode* n) {
  return isSgClassExp(n);
}

template <>
SgFunctionParameterRefExp* is<SgFunctionParameterRefExp>(SgNode* n) {
  return isSgFunctionParameterRefExp(n);
}

template <>
SgLambdaExp* is<SgLambdaExp>(SgNode* n) {
  return isSgLambdaExp(n);
}

template <>
SgHereExp* is<SgHereExp>(SgNode* n) {
  return isSgHereExp(n);
}

template <>
SgAtExp* is<SgAtExp>(SgNode* n) {
  return isSgAtExp(n);
}

template <>
SgFinishExp* is<SgFinishExp>(SgNode* n) {
  return isSgFinishExp(n);
}

template <>
SgNoexceptOp* is<SgNoexceptOp>(SgNode* n) {
  return isSgNoexceptOp(n);
}

template <>
SgNonrealRefExp* is<SgNonrealRefExp>(SgNode* n) {
  return isSgNonrealRefExp(n);
}

template <>
SgAdaTaskRefExp* is<SgAdaTaskRefExp>(SgNode* n) {
  return isSgAdaTaskRefExp(n);
}

template <>
SgAdaProtectedRefExp* is<SgAdaProtectedRefExp>(SgNode* n) {
  return isSgAdaProtectedRefExp(n);
}

template <>
SgFoldExpression* is<SgFoldExpression>(SgNode* n) {
  return isSgFoldExpression(n);
}

template <>
SgAwaitExpression* is<SgAwaitExpression>(SgNode* n) {
  return isSgAwaitExpression(n);
}

template <>
SgChooseExpression* is<SgChooseExpression>(SgNode* n) {
  return isSgChooseExpression(n);
}

template <>
SgAdaAttributeExp* is<SgAdaAttributeExp>(SgNode* n) {
  return isSgAdaAttributeExp(n);
}

template <>
SgJovialTablePresetExp* is<SgJovialTablePresetExp>(SgNode* n) {
  return isSgJovialTablePresetExp(n);
}

template <>
SgJovialPresetPositionExp* is<SgJovialPresetPositionExp>(SgNode* n) {
  return isSgJovialPresetPositionExp(n);
}

template <>
SgAdaOthersExp* is<SgAdaOthersExp>(SgNode* n) {
  return isSgAdaOthersExp(n);
}

template <>
SgAdaRenamingRefExp* is<SgAdaRenamingRefExp>(SgNode* n) {
  return isSgAdaRenamingRefExp(n);
}

template <>
SgAdaUnitRefExp* is<SgAdaUnitRefExp>(SgNode* n) {
  return isSgAdaUnitRefExp(n);
}

template <>
SgSymbol* is<SgSymbol>(SgNode* n) {
  return isSgSymbol(n);
}

template <>
SgVariableSymbol* is<SgVariableSymbol>(SgNode* n) {
  return isSgVariableSymbol(n);
}

template <>
SgTemplateVariableSymbol* is<SgTemplateVariableSymbol>(SgNode* n) {
  return isSgTemplateVariableSymbol(n);
}

template <>
SgNonrealSymbol* is<SgNonrealSymbol>(SgNode* n) {
  return isSgNonrealSymbol(n);
}

template <>
SgFunctionSymbol* is<SgFunctionSymbol>(SgNode* n) {
  return isSgFunctionSymbol(n);
}

template <>
SgMemberFunctionSymbol* is<SgMemberFunctionSymbol>(SgNode* n) {
  return isSgMemberFunctionSymbol(n);
}

template <>
SgTemplateMemberFunctionSymbol* is<SgTemplateMemberFunctionSymbol>(SgNode* n) {
  return isSgTemplateMemberFunctionSymbol(n);
}

template <>
SgTemplateFunctionSymbol* is<SgTemplateFunctionSymbol>(SgNode* n) {
  return isSgTemplateFunctionSymbol(n);
}

template <>
SgRenameSymbol* is<SgRenameSymbol>(SgNode* n) {
  return isSgRenameSymbol(n);
}

template <>
SgAdaInheritedFunctionSymbol* is<SgAdaInheritedFunctionSymbol>(SgNode* n) {
  return isSgAdaInheritedFunctionSymbol(n);
}

template <>
SgFunctionTypeSymbol* is<SgFunctionTypeSymbol>(SgNode* n) {
  return isSgFunctionTypeSymbol(n);
}

template <>
SgClassSymbol* is<SgClassSymbol>(SgNode* n) {
  return isSgClassSymbol(n);
}

template <>
SgTemplateClassSymbol* is<SgTemplateClassSymbol>(SgNode* n) {
  return isSgTemplateClassSymbol(n);
}

template <>
SgTemplateSymbol* is<SgTemplateSymbol>(SgNode* n) {
  return isSgTemplateSymbol(n);
}

template <>
SgEnumSymbol* is<SgEnumSymbol>(SgNode* n) {
  return isSgEnumSymbol(n);
}

template <>
SgEnumFieldSymbol* is<SgEnumFieldSymbol>(SgNode* n) {
  return isSgEnumFieldSymbol(n);
}

template <>
SgTypedefSymbol* is<SgTypedefSymbol>(SgNode* n) {
  return isSgTypedefSymbol(n);
}

template <>
SgTemplateTypedefSymbol* is<SgTemplateTypedefSymbol>(SgNode* n) {
  return isSgTemplateTypedefSymbol(n);
}

template <>
SgLabelSymbol* is<SgLabelSymbol>(SgNode* n) {
  return isSgLabelSymbol(n);
}

template <>
SgDefaultSymbol* is<SgDefaultSymbol>(SgNode* n) {
  return isSgDefaultSymbol(n);
}

template <>
SgNamespaceSymbol* is<SgNamespaceSymbol>(SgNode* n) {
  return isSgNamespaceSymbol(n);
}

template <>
SgIntrinsicSymbol* is<SgIntrinsicSymbol>(SgNode* n) {
  return isSgIntrinsicSymbol(n);
}

template <>
SgModuleSymbol* is<SgModuleSymbol>(SgNode* n) {
  return isSgModuleSymbol(n);
}

template <>
SgInterfaceSymbol* is<SgInterfaceSymbol>(SgNode* n) {
  return isSgInterfaceSymbol(n);
}

template <>
SgCommonSymbol* is<SgCommonSymbol>(SgNode* n) {
  return isSgCommonSymbol(n);
}

template <>
SgAliasSymbol* is<SgAliasSymbol>(SgNode* n) {
  return isSgAliasSymbol(n);
}

template <>
SgJavaLabelSymbol* is<SgJavaLabelSymbol>(SgNode* n) {
  return isSgJavaLabelSymbol(n);
}

template <>
SgAdaPackageSymbol* is<SgAdaPackageSymbol>(SgNode* n) {
  return isSgAdaPackageSymbol(n);
}

template <>
SgAdaTaskSymbol* is<SgAdaTaskSymbol>(SgNode* n) {
  return isSgAdaTaskSymbol(n);
}

template <>
SgAdaProtectedSymbol* is<SgAdaProtectedSymbol>(SgNode* n) {
  return isSgAdaProtectedSymbol(n);
}

template <>
SgAdaRenamingSymbol* is<SgAdaRenamingSymbol>(SgNode* n) {
  return isSgAdaRenamingSymbol(n);
}

template <>
SgAdaGenericSymbol* is<SgAdaGenericSymbol>(SgNode* n) {
  return isSgAdaGenericSymbol(n);
}

template <>
SgAdaGenericInstanceSymbol* is<SgAdaGenericInstanceSymbol>(SgNode* n) {
  return isSgAdaGenericInstanceSymbol(n);
}

template <>
SgAdaFormalPackageSymbol* is<SgAdaFormalPackageSymbol>(SgNode* n) {
  return isSgAdaFormalPackageSymbol(n);
}

#endif  // IS_TYPE_ROSE_HPP