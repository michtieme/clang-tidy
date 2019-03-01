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

 auto constructor = cxxConstructExpr(hasDeclaration(cxxMethodDecl(hasName("QString"))), 
		 		     hasArgument(0, stringLiteral().bind("lit")),
				     unless(anyOf(hasArgument(0, callExpr(hasDescendant(declRefExpr(to(functionDecl(hasName("tr"))))))), 
						  hasArgument(0, callExpr(hasDescendant(declRefExpr(to(functionDecl(hasName("no_tr")))))))))).bind("constructor");

  Finder->addMatcher(constructor, this);
}

void QstringsNeedTrCheck::check(const MatchFinder::MatchResult &Result) {

	const auto *matchedConstructor = Result.Nodes.getNodeAs<StringLiteral>("lit");

	if(matchedConstructor->getBeginLoc().isMacroID())
		return;

	//Produce diagnostic if we have a QString constructor match ,with a string literal, that is not wrapped in a tr(), or a no_tr()
	if(matchedConstructor)
	{
		const StringRef bytes = matchedConstructor->getBytes();
		std::string Replacement = (R"(tr(")" + bytes + R"("))").str();

		CharSourceRange CharRange = Lexer::makeFileCharRange(
			CharSourceRange::getTokenRange(matchedConstructor->getSourceRange()),
			*Result.SourceManager, 
			getLangOpts());

		SourceLocation loc = matchedConstructor->getBeginLoc();
		diag(loc, "QString should have a tr() to ensure it is translated")
		<< FixItHint::CreateReplacement(CharRange, Replacement);
	}
}

} // namespace misc
} // namespace tidy
} // namespace clang
