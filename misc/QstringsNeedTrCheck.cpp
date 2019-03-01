//===--- QstringsNeedTrCheck.cpp - clang-tidy -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "QstringsNeedTrCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace misc {

void QstringsNeedTrCheck::registerMatchers(MatchFinder *Finder) {
  if(!getLangOpts().CPlusPlus)
  {
	 return;
  }

  std::cout << "QStringsNeedTrCheck::registering Matchers" << "\n";

//  auto cxWithTr = cxxConstructExpr(hasDeclaration(cxxMethodDecl(hasName("QString"))), hasArgument(0, callExpr(hasDescendant(declRefExpr(to(functionDecl(hasName("tr")))))))).bind("cxWithTr");
//  auto cxWithNoTr = cxxConstructExpr(hasDeclaration(cxxMethodDecl(hasName("QString"))), hasArgument(0, callExpr(hasDescendant(declRefExpr(to(functionDecl(hasName("no_tr")))))))).bind("cxWithNoTr");
//  auto constructor = cxxConstructExpr(hasDeclaration(cxxMethodDecl(hasName("QString")))).bind("constructor");

  auto constructor = cxxConstructExpr(hasDeclaration(cxxMethodDecl(hasName("QString"))), 
		  unless(anyOf(hasArgument(0, callExpr(hasDescendant(declRefExpr(to(functionDecl(hasName("tr"))))))),
		               hasArgument(0, callExpr(hasDescendant(declRefExpr(to(functionDecl(hasName("no_tr")))))))
			  )))
	  .bind("constructor");

  Finder->addMatcher(constructor, this);
}

void QstringsNeedTrCheck::check(const MatchFinder::MatchResult &Result) {

	const auto *matchedConstructor = Result.Nodes.getNodeAs<CXXConstructExpr>("constructor");

	//Produce diagnostic if we have a QString constructor match that is not wrapped in a tr(), or a no_tr()
	if(matchedConstructor)
	{
		SourceLocation loc = matchedConstructor->getBeginLoc();
		diag(loc, "QString should have a tr() to ensure it is translated")
      		<< FixItHint::CreateInsertion(matchedConstructor->getLocation(), "tr()");
	}

	// FIXME: Add callback implementation.
//  const auto *MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("x");
//  if (MatchedDecl->getName().startswith("awesome_"))
//    return;
//  diag(MatchedDecl->getLocation(), "function %0 is insufficiently awesome")
//      << MatchedDecl
//      << FixItHint::CreateInsertion(MatchedDecl->getLocation(), "awesome_");
}

} // namespace misc
} // namespace tidy
} // namespace clang
