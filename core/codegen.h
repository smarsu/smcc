// Copyright (c) 2020 smarsu. All Rights Reserved.

#pragma once
#include "core/ast.h"

namespace smcc {

class CodeGen {
 public:
  CodeGen(const AST &ast) : ast_(ast) {
    VM2();
  }

  void VM();

  void VM2();

 private:
  AST ast_;
};

}  // namespace smcc
