// Copyright (c) 2020 smarsu. All Rights Reserved.

#pragma once
#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <cstring>

class ExprAST {
 public:
  virtual ~ExprAST() {}

  virtual void codegen() = 0;

  virtual void *ret_val() { return ret_val_; }

  virtual size_t val_size() { return val_size_; }

  virtual const std::string &get_name() { return __name_; };

  size_t byteof(const std::string &type) {
    if (type == "int") {
      return 4;
    }
    if (type == "double") {
      return 8;
    }
    return -1;
  }

 public:
  void reshape(size_t size) { 
    if (size > capacity_) {
      if (ret_val_) {
        free(ret_val_);
      }
      val_size_ = size;
      capacity_ = size;
      ret_val_ = malloc(val_size_);
    }
    else {
      val_size_ = size;
    }
  }

  void feed(ExprAST *other) {
    fprintf(stderr, "%ld %ld\n", other->val_size(), val_size_);
    assert (other->val_size() == val_size_);
    memcpy(ret_val_, other->ret_val(), val_size_);
  }

  void feed(const void *other) {
    memcpy(ret_val_, other, val_size_);
  }

 protected:
  void *ret_val_{nullptr};
  size_t val_size_{0};
  size_t capacity_{0};
  std::string __name_{0};
};

class IncludeExprAST : public ExprAST {
 public:
  IncludeExprAST(const std::string &path) : path_{path} {
    fprintf(stderr, "Parse IncludeExprAST\n");
  }

  virtual void codegen();

 private:
  std::string path_;
};

class PrototypeAST : public ExprAST {
 public:
  PrototypeAST(const std::string &type, const std::string &name, std::vector<std::unique_ptr<ExprAST>> args)
    : type_(type), name_(name), args_(std::move(args)) {
    fprintf(stderr, "Parse PrototypeAST\n");
  }

  const std::string &get_name() const { return name_; }

  const std::string &get_type() const { return type_; }

  std::vector<std::unique_ptr<ExprAST>> &get_args() { return args_; }

  virtual void codegen();

 private:
  std::string type_;
  std::string name_;
  std::vector<std::unique_ptr<ExprAST>> args_;
};

class FunctionAST : public ExprAST {
 public:
  FunctionAST(std::unique_ptr<ExprAST> proto, std::vector<std::unique_ptr<ExprAST>> body)
    : proto_(std::move(proto)), body_(std::move(body)) {
    fprintf(stderr, "Parse FunctionAST\n");
    if (get_proto()) {
      reshape(byteof(get_proto()->get_type()));
    }
  }

  virtual void codegen();

  virtual void set_params(const std::vector<void *> &params);

  virtual PrototypeAST *get_proto() { return reinterpret_cast<PrototypeAST *>(proto_.get()); }

  virtual std::vector<std::unique_ptr<ExprAST>> &get_body() { return body_; }

  virtual void run(std::vector<std::unique_ptr<ExprAST>> &params) {
    for (auto &expr : body_) {
      expr->codegen();
    }
  }

 private:
  std::unique_ptr<ExprAST> proto_;
  std::vector<std::unique_ptr<ExprAST>> body_;
};

class CallExprAST : public ExprAST {
 public:
  CallExprAST(const std::string &callee, std::vector<std::unique_ptr<ExprAST>> args)
    : callee_(callee), args_(std::move(args)) {
    fprintf(stderr, "Parse CallExprAST\n");
  }

  virtual void codegen();

 private:
  std::string callee_;
  std::vector<std::unique_ptr<ExprAST>> args_;
};

class ReturnExprAST : public ExprAST {
 public:
  ReturnExprAST(std::unique_ptr<ExprAST> ret)
    : ret_(std::move(ret)) {
    fprintf(stderr, "Parse ReturnExprAST\n");
  }

  virtual void codegen();
  
 private:
  std::unique_ptr<ExprAST> ret_;
};

class StrExprAST : public ExprAST {
 public:
  StrExprAST(const std::string &val) : val_(val) {
    fprintf(stderr, "Parse StrExprAST\n");
  }

  virtual void codegen();

 private:
  std::string val_;
};

class NumberExprAST : public ExprAST {
 public:
  NumberExprAST(double val) : val_(val) {
    fprintf(stderr, "Parse NumberExprAST\n");
  }

  virtual void codegen();

 private:
  double val_;
};

class VariableExprAST : public ExprAST {
 public:
  VariableExprAST(const std::string &type, const std::string &name) 
    : type_(type), name_(name) {
    fprintf(stderr, "Parse VariableExprAST\n");
  }

  virtual void codegen();

 private:
  std::string type_;
  std::string name_;
};

class BinaryExprAST : public ExprAST {
 public:
  BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS)
    : op_(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {
    fprintf(stderr, "Parse BinaryExprAST\n");
  }

  virtual void codegen();

 private:
  char op_;
  std::unique_ptr<ExprAST> LHS, RHS;
};

class PrintExprAST : public FunctionAST {
 public:
  PrintExprAST(std::unique_ptr<ExprAST> proto, std::vector<std::unique_ptr<ExprAST>> body) 
  : FunctionAST(std::move(proto), std::move(body)) {}

  virtual void codegen() {}

  virtual void set_params(const std::vector<void *> &params) {}

  virtual PrototypeAST *get_proto() { return reinterpret_cast<PrototypeAST *>(proto_.get()); }

  virtual std::vector<std::unique_ptr<ExprAST>> &get_body() { return body_; }

  virtual void run(std::vector<std::unique_ptr<ExprAST>> &params) {
    printf((char *)(params[0]->ret_val()));
  }

 private:
  std::unique_ptr<ExprAST> proto_;
  std::vector<std::unique_ptr<ExprAST>> body_;
};
