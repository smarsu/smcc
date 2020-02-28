// Copyright (c) 2020 smarsu. All Rights Reserved.

#pragma once
#include <cstring>
#include <vector>
#include <string>
#include <map>
#include <tuple>

namespace smcc {

enum class TokenType : int {
  kInvalid = 0,
  kGlobal,
  kList,
  kFunc,
  kVar,
  kAssign,
  kConstInt,
  kConstFloat,
  kPrint,
  kConstStr,
  kAdd,
  kMul,
  kSub,
  kDiv,
  kGreatThan,
  kIf,
  kLessThan,
  kWhile,
  kDef,
  kReturn,
  kArray,
};

enum class DataType : int{
  kInvalid = 0,
  kInt,
  kFloat,
  kStr,
  kArray,
};

class Token;
Token *ToToken(const std::string &name);

// Token *ToToken(const std::string &name) {
//   Token *token = nullptr;

//   if (name == "=") {
//     token = new AssignToken(name);
//   }
//   else if (name == "if") {
//     token = new IfToken(name);
//   }
//   else if (name == "<") {
//     token = new LessThanToken(name);
//   }
//   else if (name == ":") {
//     token = new ColonToken(name);
//   }
//   else if (name == "print") {
//     token = new PrintToken(name);
//   }
//   else if (name == "(") {
//     token = new LeftP1Token(name);
//   }
//   else if (name == ")") {
//     token = new RightP1Token(name);
//   }
//   else if (IsVar(name)) {
//     token = new VarToken(name);
//   }
//   else {
//     LOG(FLATA) << "Unexpected token " << name;
//   }

//   CHECK_NOTNULL(token);
//   return token;
// }

class Token {
 public:
  Token(const std::string &name, TokenType token_type) 
   : name_(name), token_type_{token_type} {
    var_ = new void*;
    size_ = new size_t;
    data_type_ = new DataType;
  }

  virtual void Prepare() {};

  virtual void Run() {};

  void Resize(size_t size) {
    if (size == *size_) return;
    if (*var_) {
      // delete *var_; 
    }
    *var_ = new char[size];
    *size_ = size;
  }

  void SetVar(const void *ptr, size_t size, DataType data_type) {
    Resize(size);
    std::memcpy(*var_, ptr, size);
    *data_type_ = data_type;
  }

  void SetVar(Token *other) {
    SetVar(other->var(), other->size(), other->data_type());
  }

  ~Token() {}

 public:
  void *var() { return *var_; }

  size_t size() { return *size_; }

  DataType data_type() { return *data_type_; }

  TokenType token_type() { return token_type_; }

  std::string repr();


  Token *left(size_t offset = 0) {
    size_t k = 0;
    for (; k < father_->kids().size(); ++k) {
      if (father_->kids()[k] == this) {
        break;
      }
    }

    return father_->kids()[k - 1 - offset];
  }

 public:
  void push(Token *node) { 
    kids_.push_back(node); 
    node->SetFather(this);
  };

  Token *pop();

  void SetFather(Token *father) { father_ = father; }

 public:
  std::vector<Token *> &kids() { return kids_; }

  Token *father() { return father_; }

  std::string name() { return name_; }

  void DebugString(int deep = 0, int level = 0);

 protected:
  Token *father_{nullptr};
  std::vector<Token *> kids_;
  std::string name_;

  TokenType token_type_{TokenType::kInvalid};

 protected:
  void **var_{nullptr};
  size_t *size_{nullptr};
  DataType *data_type_{nullptr};
};

class GlobalToken : public Token {
 public:
  GlobalToken(const std::string &name) : Token(name, TokenType::kGlobal) {}

  virtual void Run() override;
};

class ListToken : public Token {
 public:
  ListToken(const std::string &name) : Token(name, TokenType::kList) {}

  virtual void Run() override;
};

class FuncToken : public Token {
 public:
  FuncToken(const std::string &name) : Token(name, TokenType::kFunc) {}

  virtual void Run() override;
};

class VarToken : public Token {
 public:
  VarToken(const std::string &name) : Token(name, TokenType::kVar) {}

  virtual void Prepare() override;

  virtual void Run() override;
};

class AssignToken : public Token {
 public:
  AssignToken(const std::string &name) : Token(name, TokenType::kAssign) {}

  virtual void Run() override;
};

class ConstIntToken : public Token {
 public:
  ConstIntToken(const std::string &name) : Token(name, TokenType::kConstInt) {}

  virtual void Prepare() override;
};

class ConstFloatToken : public Token {
 public:
  ConstFloatToken(const std::string &name) : Token(name, TokenType::kConstFloat) {}

  virtual void Prepare() override;
};

class PrintToken : public Token {
 public:
  PrintToken(const std::string &name) : Token(name, TokenType::kPrint) {}

  virtual void Run() override;
};

class ConstStrToken : public Token {
 public:
  ConstStrToken(const std::string &name) : Token(name, TokenType::kConstStr) {}

  virtual void Prepare() override;
};

class AddToken : public Token {
 public:
  AddToken(const std::string &name) : Token(name, TokenType::kAdd) {}

  virtual void Run() override;
};

class MulToken : public Token {
 public:
  MulToken(const std::string &name) : Token(name, TokenType::kMul) {}

  virtual void Run() override;
};

class SubToken : public Token {
 public:
  SubToken(const std::string &name) : Token(name, TokenType::kSub) {}

  virtual void Run() override;
};

class DivToken : public Token {
 public:
  DivToken(const std::string &name) : Token(name, TokenType::kDiv) {}

  virtual void Run() override;
};

class GreatThanToken : public Token {
 public:
  GreatThanToken(const std::string &name) : Token(name, TokenType::kGreatThan) {}

  virtual void Run() override;
};

class IfToken : public Token {
 public:
  IfToken(const std::string &name) : Token(name, TokenType::kIf) {}

  virtual void Run() override;
};

class LessThanToken :public Token {
 public:
  LessThanToken(const std::string &name) : Token(name, TokenType::kLessThan) {}

  virtual void Run() override;
};

class WhileToken : public Token {
 public:
  WhileToken(const std::string &name) : Token(name, TokenType::kWhile) {}

  virtual void Run() override;  
};

class DefToken : public Token {
 public:
  DefToken(const std::string &name) : Token(name, TokenType::kDef) {}

  virtual void Prepare() override;
};

class ReturnToken : public Token {
 public:
  ReturnToken(const std::string &name) : Token(name, TokenType::kReturn) {}

  virtual void Run() override;
};

class ArrayToken : public Token {
 public:
  ArrayToken(const std::string &name) : Token(name, TokenType::kArray) {}

  virtual void Run() override;
};

};
