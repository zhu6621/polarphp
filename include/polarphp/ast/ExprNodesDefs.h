//===--- ExprNodes.def - Swift Expression AST Metaprogramming ---*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2017 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
// This source file is part of the polarphp.org open source project
//
// Copyright (c) 2017 - 2018 polarphp software foundation
// Copyright (c) 2017 - 2018 zzu_softboy <zzu_softboy@163.com>
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://polarphp.org/LICENSE.txt for license information
// See https://polarphp.org/CONTRIBUTORS.txt for the list of polarphp project authors
//
// Created by polarboy on 2019/04/25.
//
// This file defines macros used for macro-metaprogramming with expressions.
//
//===----------------------------------------------------------------------===//

/// EXPR(Id, Parent)
///   If the expression node is not abstract, its enumerator value is
///   ExprKind::Id.  The node's class name is Id##Expr, and the name of
///   its base class (in the Expr hierarchy) is Parent.
#ifndef EXPR
#define EXPR(Id, Parent)
#endif

/// An abstract expression node is an abstract base class in the hierarchy;
/// it is never a most-derived type, and it does not have an enumerator in
/// ExprKind.
///
/// Most metaprograms do not care about abstract expressions, so the default
/// is to ignore them.
#ifndef ABSTRACT_EXPR
#define ABSTRACT_EXPR(Id, Parent)
#endif

/// An "unchecked" expression node is removed from valid code by the end
/// of the type-checking phase.
///
/// By default, these are treated like any other expression.
#ifndef UNCHECKED_EXPR
#define UNCHECKED_EXPR(Id, Parent) EXPR(Id, Parent)
#endif

/// A literal expression node represents a literal value, such as a number,
/// boolean, string, etc.
///
/// By default, these are treated like any other expression.
#ifndef LITERAL_EXPR
#define LITERAL_EXPR(Id, Parent) EXPR(Id, Parent)
#endif

/// A expression node with a DeclContext. For example: closures.
///
/// By default, these are treated like any other expression.
#ifndef CONTEXT_EXPR
#define CONTEXT_EXPR(Id, Parent) EXPR(Id, Parent)
#endif

/// A convenience for determining the range of expressions.  These will always
/// appear immediately after the last member.
#ifndef EXPR_RANGE
#define EXPR_RANGE(Id, First, Last)
#endif

#ifndef LAST_EXPR
#define LAST_EXPR(Id)
#endif

EXPR(Error, Expr)
ABSTRACT_EXPR(Literal, Expr)
  LITERAL_EXPR(NilLiteral, LiteralExpr)
  ABSTRACT_EXPR(NumberLiteral, LiteralExpr)
    LITERAL_EXPR(IntegerLiteral, NumberLiteralExpr)
    LITERAL_EXPR(FloatLiteral, NumberLiteralExpr)
  EXPR_RANGE(NumberLiteral, IntegerLiteral, FloatLiteral)
  LITERAL_EXPR(BooleanLiteral, LiteralExpr)
  LITERAL_EXPR(StringLiteral, LiteralExpr)
  LITERAL_EXPR(InterpolatedStringLiteral, LiteralExpr)
  LITERAL_EXPR(ObjectLiteral, LiteralExpr)
  LITERAL_EXPR(MagicIdentifierLiteral, LiteralExpr)
  EXPR_RANGE(Literal, NilLiteral, MagicIdentifierLiteral)
EXPR(DiscardAssignment, Expr)
EXPR(DeclRef, Expr)
EXPR(SuperRef, Expr)
EXPR(Type, Expr)
EXPR(OtherConstructorDeclRef, Expr)
EXPR(DotSyntaxBaseIgnored, Expr)
ABSTRACT_EXPR(OverloadSetRef, Expr)
  UNCHECKED_EXPR(OverloadedDeclRef, OverloadSetRefExpr)
  EXPR_RANGE(OverloadSetRef, OverloadedDeclRef, OverloadedDeclRef)
UNCHECKED_EXPR(UnresolvedDeclRef, Expr)
ABSTRACT_EXPR(Lookup, Expr)
  EXPR(MemberRef, LookupExpr)
  EXPR(Subscript, LookupExpr)
  ABSTRACT_EXPR(DynamicLookup, LookupExpr)
    EXPR(DynamicMemberRef, DynamicLookupExpr)
    EXPR(DynamicSubscript, DynamicLookupExpr)
    EXPR_RANGE(DynamicLookup, DynamicMemberRef, DynamicSubscript)
  EXPR_RANGE(Lookup, MemberRef, DynamicSubscript)
UNCHECKED_EXPR(UnresolvedSpecialize, Expr)
UNCHECKED_EXPR(UnresolvedMember, Expr)
UNCHECKED_EXPR(UnresolvedDot, Expr)
UNCHECKED_EXPR(Sequence, Expr)
ABSTRACT_EXPR(Identity, Expr)
  EXPR(Paren, IdentityExpr)
  EXPR(DotSelf, IdentityExpr)
  EXPR_RANGE(Identity, Paren, DotSelf)
ABSTRACT_EXPR(AnyTry, Expr)
  EXPR(Try, AnyTryExpr)
  EXPR(ForceTry, AnyTryExpr)
  EXPR(OptionalTry, AnyTryExpr)
  EXPR_RANGE(AnyTry, Try, OptionalTry)
EXPR(Tuple, Expr)
ABSTRACT_EXPR(Collection, Expr)
  EXPR(Array, CollectionExpr)
  EXPR(Dictionary, CollectionExpr)
  EXPR_RANGE(Collection, Array, Dictionary)
EXPR(KeyPathApplication, Expr)
EXPR(TupleElement, Expr)
EXPR(CaptureList, Expr)
ABSTRACT_EXPR(AbstractClosure, Expr)
  CONTEXT_EXPR(Closure, AbstractClosureExpr)
  CONTEXT_EXPR(AutoClosure, AbstractClosureExpr)
  EXPR_RANGE(AbstractClosure, Closure, AutoClosure)
EXPR(InOut, Expr)
EXPR(VarargExpansion, Expr)
EXPR(DynamicType, Expr)
EXPR(RebindSelfInConstructor, Expr)
EXPR(OpaqueValue, Expr)
EXPR(BindOptional, Expr)
EXPR(OptionalEvaluation, Expr)
EXPR(ForceValue, Expr)
EXPR(OpenExistential, Expr)
EXPR(MakeTemporarilyEscapable, Expr)
ABSTRACT_EXPR(Apply, Expr)
  EXPR(Call, ApplyExpr)
  EXPR(PrefixUnary, ApplyExpr)
  EXPR(PostfixUnary, ApplyExpr)
  EXPR(Binary, ApplyExpr)
  ABSTRACT_EXPR(SelfApply, ApplyExpr)
    EXPR(DotSyntaxCall, SelfApplyExpr)
    EXPR(ConstructorRefCall, SelfApplyExpr)
    EXPR_RANGE(SelfApply, DotSyntaxCall, ConstructorRefCall)
  EXPR_RANGE(Apply, Call, ConstructorRefCall)
ABSTRACT_EXPR(ImplicitConversion, Expr)
  EXPR(Load, ImplicitConversionExpr)
  EXPR(TupleShuffle, ImplicitConversionExpr)
  EXPR(UnresolvedTypeConversion, ImplicitConversionExpr)
  EXPR(FunctionConversion, ImplicitConversionExpr)
  EXPR(CovariantFunctionConversion, ImplicitConversionExpr)
  EXPR(CovariantReturnConversion, ImplicitConversionExpr)
  EXPR(ImplicitlyUnwrappedFunctionConversion, ImplicitConversionExpr)
  EXPR(MetatypeConversion, ImplicitConversionExpr)
  EXPR(CollectionUpcastConversion, ImplicitConversionExpr)
  EXPR(Erasure, ImplicitConversionExpr)
  EXPR(AnyHashableErasure, ImplicitConversionExpr)
  EXPR(BridgeToObjC, ImplicitConversionExpr)
  EXPR(BridgeFromObjC, ImplicitConversionExpr)
  EXPR(ConditionalBridgeFromObjC, ImplicitConversionExpr)
  EXPR(DerivedToBase, ImplicitConversionExpr)
  EXPR(ArchetypeToSuper, ImplicitConversionExpr)
  EXPR(InjectIntoOptional, ImplicitConversionExpr)
  EXPR(ClassMetatypeToObject, ImplicitConversionExpr)
  EXPR(ExistentialMetatypeToObject, ImplicitConversionExpr)
  EXPR(ProtocolMetatypeToObject, ImplicitConversionExpr)
  EXPR(InOutToPointer, ImplicitConversionExpr)
  EXPR(ArrayToPointer, ImplicitConversionExpr)
  EXPR(StringToPointer, ImplicitConversionExpr)
  EXPR(PointerToPointer, ImplicitConversionExpr)
  EXPR(ForeignObjectConversion, ImplicitConversionExpr)
  EXPR(UnevaluatedInstance, ImplicitConversionExpr)
  EXPR_RANGE(ImplicitConversion, Load, UnevaluatedInstance)
ABSTRACT_EXPR(ExplicitCast, Expr)
  ABSTRACT_EXPR(CheckedCast, ExplicitCastExpr)
    EXPR(ForcedCheckedCast, CheckedCastExpr)
    EXPR(ConditionalCheckedCast, CheckedCastExpr)
    EXPR(Is, CheckedCastExpr)
    EXPR_RANGE(CheckedCast, ForcedCheckedCast, Is)
  EXPR(Coerce, ExplicitCastExpr)
  EXPR_RANGE(ExplicitCast, ForcedCheckedCast, Coerce)
UNCHECKED_EXPR(Arrow, Expr)
EXPR(If, Expr)
EXPR(EnumIsCase, Expr)
EXPR(Assign, Expr)
EXPR(CodeCompletion, Expr)
UNCHECKED_EXPR(UnresolvedPattern, Expr)
EXPR(LazyInitializer, Expr)
EXPR(EditorPlaceholder, Expr)
EXPR(ObjCSelector, Expr)
EXPR(KeyPath, Expr)
UNCHECKED_EXPR(KeyPathDot, Expr)
EXPR(Tap, Expr)
LAST_EXPR(Tap)

#undef EXPR_RANGE
#undef LITERAL_EXPR
#undef UNCHECKED_EXPR
#undef ABSTRACT_EXPR
#undef CONTEXT_EXPR
#undef EXPR
#undef LAST_EXPR
