// Copyright (c) 2020 smarsu. All Rights Reserved.

#include <string>

#include "core/node.h"
#include "core/logging.h"

namespace smcc {

Node *Node::pop() {
  CHECK_GT(kids_.size(), 0);
  Node *kid = kids_[kids_.size() - 1];
  kids_.resize(kids_.size() - 1);

  kid->SetFather(nullptr);
  return kid;
}

void Node::DebugString(int deep, int level) {
  std::string prefix(deep * 2, ' ');
  std::string s = prefix + name_;
  if (level == 1) {
    s += " ";
    s += std::to_string(*value<double>());
  }
  if (level == 2) {
    s += " ";
    s += std::to_string(*value<double>());
    s += " ";
    s += std::to_string(reinterpret_cast<uint64_t>(value<void>()));
  }
  std::cout << s << std::endl;
  for (auto *node : kids_) {
    node->DebugString(deep + 1, level);
  }
}

}  // namespace smcc
