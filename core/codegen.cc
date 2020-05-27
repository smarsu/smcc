// Copyright (c) 2020 smarsu. All Rights Reserved.

#include <map>

#include "core/ast.h"
#include "core/IR.h"

size_t byteof(const std::string &type) {
  if (type == "int") {
    return 4;
  }
  if (type == "double") {
    return 8;
  }
  return -1;
}

class FuncTable {
 public:
  static std::map<std::string, FunctionAST *> &func_table() {
    static std::map<std::string, FunctionAST *> func_table;
    static bool init = false;
    if (!init) {
      func_table["printf"] = new PrintExprAST(nullptr, std::vector<std::unique_ptr<ExprAST>>());
      init = true;
    }
    return func_table;
  }
};

FunctionAST *this_func = nullptr;

void ShowFuncTable() {
  for (auto table : FuncTable::func_table()) {
    fprintf(stderr, "%s: %p\n", table.first.c_str(), table.second);
  }
}

void IncludeExprAST::codegen() {
  // TODO(smarsu)
}

void FunctionAST::set_params(const std::vector<void *> &params) {
  PrototypeAST *proto = get_proto();
  for (size_t i = 0; i < params.size(); ++i) {
    proto->get_args()[i]->feed(params[i]);
  }
}

void FunctionAST::codegen() {
  FuncTable::func_table()[get_proto()->get_name()] = this;
  // proto_->codegen();
  // for (auto &body : body_) {
  //   body->codegen();
  // }
}

void CallExprAST::codegen() {
  ShowFuncTable();
  auto last_func = this_func;
  this_func = FuncTable::func_table()[callee_];

  std::vector<void *> arg_vals;
  for (auto &arg : args_) {
    arg->codegen();
    arg_vals.push_back(arg->ret_val());
  }
  this_func->set_params(arg_vals);
  this_func->run(args_);

  this_func = last_func;
}

void StrExprAST::codegen() {
  val_size_ = val_.size();
  fprintf(stderr, "val_size_: %ld\n", val_size_);
  ret_val_ = malloc(val_size_);
  memcpy(ret_val_, val_.c_str(), val_size_);
}

void NumberExprAST::codegen() {
  val_size_ = sizeof(double);
  ret_val_ = malloc(val_size_);
  memcpy(ret_val_, &val_, val_size_);
}

void ReturnExprAST::codegen() {
  ret_->codegen();

  auto type = this_func->get_proto()->get_type();
  this_func->reshape(byteof(type));
  this_func->feed(ret_.get());
}

void VariableExprAST::codegen() {
  
}

void BinaryExprAST::codegen() {
  
}

void PrototypeAST::codegen() {
  
}
