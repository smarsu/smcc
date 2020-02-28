// Copyright (c) 2020 smarsu. All Rights Reserved.

#pragma once
#include <set>
#include <string>

#include "core/node.h"
#include "core/token.h"

namespace smcc {

class AST {
 public:
  AST() {}

  AST(const std::string &filename, bool S = false) 
   : filename_(filename), tree_(new Node(filename)) {
    ParseV2(filename_, S);
  }

  Node *tree() { return tree_; }

  Token *tree_v2() { return global_; }

  ~AST() {
    // if (tree_) {
    //   delete tree_;
    // }
  }

 private:
  void Parse(const std::string &filename, bool S = false);

  void ParseV2(const std::string &filename, bool S = false);

 private:
  std::string filename_;
  Node *tree_{nullptr};
  Token *global_{nullptr};
};

}  // namespace smcc
