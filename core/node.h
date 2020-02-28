// Copyright (c) 2020 smarsu. All Rights Reserved.

#pragma once
#include <vector>

namespace smcc {

enum class Type : int {
  kInt,
  kFloat,
  kString,
};
 
class Node {
 public:
  Node(const std::string &name) : name_(name) {
    value_ = malloc(sizeof(int64_t));
  }

  void push(Node *node) { 
    kids_.push_back(node); 
    node->SetFather(this);
  };

  Node *pop();

  void SetFather(Node *father) { father_ = father; }

  void SetValue(void *value) { value_ = value; }

  void SetType(Type type) { type_ = type; }

  template <typename T>
  T *value() { return reinterpret_cast<T *>(value_); }

  std::vector<Node *> &kids() { return kids_; }

  Node *father() { return father_; }

  Type type() { return type_; }

  std::string name() const { return name_; }

  void DebugString(int deep = 0, int level = 0);

  ~Node() {
    // for (auto node : kids_) {
    //   delete node;
    // }
  }

 private:
  Type type_{Type::kFloat};
  std::string name_;
  Node *father_{nullptr};
  std::vector<Node *> kids_;
  void *value_;
};

}  // namespace smcc
