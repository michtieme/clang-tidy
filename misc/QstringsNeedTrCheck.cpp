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

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace misc {

void QstringsNeedTrCheck::registerMatchers(MatchFinder *Finder) {
 if(!getLangOpts().CPlusPlus)
 {
	 return;
 }
 auto constructor = cxxConstructExpr(hasDeclaration(cxxMethodDecl(hasName("QString")))).bind("constructor");

  // FIXME: Add matchers.
  Finder->addMatcher(constructor, this);
}

void QstringsNeedTrCheck::check(const MatchFinder::MatchResult &Result) {

      
	const auto *matchedConstructor = Result.Nodes.getNodeAs<CXXConstructExpr>("constructor");

	if(matchedConstructor)
	{
		SourceLocation loc = matchedConstructor->getBeginLoc();

		// if the constructor had a tr() call inside then there is nothing to do
		// TODO: Not sure how to write that :)
		// if( has a tr() 
		//    return
	
		diag(loc, "QString should have a tr() to ensure it is translated");

      	//	<< matchedConstructor
      	//	<< FixItHint::CreateInsertion(matchedConstructor->getLocation(), "tr()");
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
