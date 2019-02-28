//===--- VirtualShadowingCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "VirtualShadowingCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace misc {

void VirtualShadowingCheck::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  
  // Bind all function declarations to 'x'
  Finder->addMatcher(functionDecl().bind("x"), this);
}

void VirtualShadowingCheck::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  
  // Get the node for 'X' which are function declarations. If it starts with 'awesome' then we can skip.
  const auto *MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("x");
  if (MatchedDecl->getName().startswith("awesome_"))
    return;

  //Otherwise we issue a diagnostic that the function is insufficently awesome
  diag(MatchedDecl->getLocation(), "function %0 is insufficiently awesome")
      << MatchedDecl
      << FixItHint::CreateInsertion(MatchedDecl->getLocation(), "awesome_");
}

} // namespace misc
} // namespace tidy
} // namespace clang
