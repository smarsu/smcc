// Copyright (c) 2020 smarsu. All Rights Reserved.

#include <utility>

#include "core/token.h"
#include "core/logging.h"

namespace smcc {

static std::map<std::string, std::tuple<void **, size_t *, DataType *>> var_map;

std::tuple<void *, size_t, DataType> 
Add(std::tuple<void *, size_t, DataType> left, std::tuple<void *, size_t, DataType> right) {
  std::tuple<void *, size_t, DataType> ret;
  
  if (std::get<2>(left) == DataType::kInt) {
    if (std::get<2>(right) == DataType::kInt) {
      std::get<2>(ret) = DataType::kInt;
      std::get<1>(ret) = sizeof(int64_t);
      std::get<0>(ret) = new char[sizeof(int64_t)];

      int64_t data = *reinterpret_cast<int64_t *>(std::get<0>(left)) + *reinterpret_cast<int64_t *>(std::get<0>(right));
      *reinterpret_cast<int64_t *>(std::get<0>(ret)) = data;
    }
    else if (std::get<2>(right) == DataType::kFloat) {
      std::get<2>(ret) = DataType::kFloat;
      std::get<1>(ret) = sizeof(double);
      std::get<0>(ret) = new char[sizeof(double)];

      double data = *reinterpret_cast<int64_t *>(std::get<0>(left)) + *reinterpret_cast<double *>(std::get<0>(right));
      *reinterpret_cast<double *>(std::get<0>(ret)) = data;
    }
  }
  else if (std::get<2>(left) == DataType::kFloat) {
    if (std::get<2>(right) == DataType::kInt) {
      std::get<2>(ret) = DataType::kFloat;
      std::get<1>(ret) = sizeof(double);
      std::get<0>(ret) = new char[sizeof(double)];

      double data = *reinterpret_cast<double *>(std::get<0>(left)) + *reinterpret_cast<int64_t *>(std::get<0>(right));
      *reinterpret_cast<double *>(std::get<0>(ret)) = data;
    }
    else if (std::get<2>(right) == DataType::kFloat) {
      std::get<2>(ret) = DataType::kFloat;
      std::get<1>(ret) = sizeof(double);
      std::get<0>(ret) = new char[sizeof(double)];

      double data = *reinterpret_cast<double *>(std::get<0>(left)) + *reinterpret_cast<double *>(std::get<0>(right));
      *reinterpret_cast<double *>(std::get<0>(ret)) = data;
    }
  }
  else if (std::get<2>(left) == DataType::kStr) {
    if (std::get<2>(right) == DataType::kStr) {
      std::get<2>(ret) = DataType::kStr;

      size_t sizel = std::get<1>(left);
      size_t sizer = std::get<1>(right);
      std::get<1>(ret) = sizel + sizer - 2 + 1;

      void *datal = std::get<0>(left);
      void *datar = std::get<0>(right);

      std::get<0>(ret) = new char[sizel + sizer - 2 + 1];
      std::memcpy(reinterpret_cast<char *>(std::get<0>(ret)), datal, sizel - 1);
      std::memcpy(reinterpret_cast<char *>(std::get<0>(ret)) + sizel - 1, datar, sizer - 1);
      reinterpret_cast<char *>(std::get<0>(ret))[sizel + sizer - 2] = '\0';
    }
  }

  return ret;
}

std::tuple<void *, size_t, DataType> 
Sub(std::tuple<void *, size_t, DataType> left, std::tuple<void *, size_t, DataType> right) {
  std::tuple<void *, size_t, DataType> ret;
  
  if (std::get<2>(left) == DataType::kInt) {
    if (std::get<2>(right) == DataType::kInt) {
      std::get<2>(ret) = DataType::kInt;
      std::get<1>(ret) = sizeof(int64_t);
      std::get<0>(ret) = new char[sizeof(int64_t)];

      int64_t data = *reinterpret_cast<int64_t *>(std::get<0>(left)) - *reinterpret_cast<int64_t *>(std::get<0>(right));
      *reinterpret_cast<int64_t *>(std::get<0>(ret)) = data;
    }
    else if (std::get<2>(right) == DataType::kFloat) {
      std::get<2>(ret) = DataType::kFloat;
      std::get<1>(ret) = sizeof(double);
      std::get<0>(ret) = new char[sizeof(double)];

      double data = *reinterpret_cast<int64_t *>(std::get<0>(left)) - *reinterpret_cast<double *>(std::get<0>(right));
      *reinterpret_cast<double *>(std::get<0>(ret)) = data;
    }
  }
  else if (std::get<2>(left) == DataType::kFloat) {
    if (std::get<2>(right) == DataType::kInt) {
      std::get<2>(ret) = DataType::kFloat;
      std::get<1>(ret) = sizeof(double);
      std::get<0>(ret) = new char[sizeof(double)];

      double data = *reinterpret_cast<double *>(std::get<0>(left)) - *reinterpret_cast<int64_t *>(std::get<0>(right));
      *reinterpret_cast<double *>(std::get<0>(ret)) = data;
    }
    else if (std::get<2>(right) == DataType::kFloat) {
      std::get<2>(ret) = DataType::kFloat;
      std::get<1>(ret) = sizeof(double);
      std::get<0>(ret) = new char[sizeof(double)];

      double data = *reinterpret_cast<double *>(std::get<0>(left)) - *reinterpret_cast<double *>(std::get<0>(right));
      *reinterpret_cast<double *>(std::get<0>(ret)) = data;
    }
  }

  return ret;
}

std::tuple<void *, size_t, DataType> 
Mul(std::tuple<void *, size_t, DataType> left, std::tuple<void *, size_t, DataType> right) {
  std::tuple<void *, size_t, DataType> ret;
  
  if (std::get<2>(left) == DataType::kInt) {
    if (std::get<2>(right) == DataType::kInt) {
      std::get<2>(ret) = DataType::kInt;
      std::get<1>(ret) = sizeof(int64_t);
      std::get<0>(ret) = new char[sizeof(int64_t)];

      int64_t data = *reinterpret_cast<int64_t *>(std::get<0>(left)) * *reinterpret_cast<int64_t *>(std::get<0>(right));
      *reinterpret_cast<int64_t *>(std::get<0>(ret)) = data;
    }
    else if (std::get<2>(right) == DataType::kFloat) {
      std::get<2>(ret) = DataType::kFloat;
      std::get<1>(ret) = sizeof(double);
      std::get<0>(ret) = new char[sizeof(double)];

      double data = *reinterpret_cast<int64_t *>(std::get<0>(left)) * *reinterpret_cast<double *>(std::get<0>(right));
      *reinterpret_cast<double *>(std::get<0>(ret)) = data;
    }
  }
  else if (std::get<2>(left) == DataType::kFloat) {
    if (std::get<2>(right) == DataType::kInt) {
      std::get<2>(ret) = DataType::kFloat;
      std::get<1>(ret) = sizeof(double);
      std::get<0>(ret) = new char[sizeof(double)];

      double data = *reinterpret_cast<double *>(std::get<0>(left)) * *reinterpret_cast<int64_t *>(std::get<0>(right));
      *reinterpret_cast<double *>(std::get<0>(ret)) = data;
    }
    else if (std::get<2>(right) == DataType::kFloat) {
      std::get<2>(ret) = DataType::kFloat;
      std::get<1>(ret) = sizeof(double);
      std::get<0>(ret) = new char[sizeof(double)];

      double data = *reinterpret_cast<double *>(std::get<0>(left)) * *reinterpret_cast<double *>(std::get<0>(right));
      *reinterpret_cast<double *>(std::get<0>(ret)) = data;
    }
  }

  return ret;
}

std::tuple<void *, size_t, DataType> 
Div(std::tuple<void *, size_t, DataType> left, std::tuple<void *, size_t, DataType> right) {
  std::tuple<void *, size_t, DataType> ret;
  
  if (std::get<2>(left) == DataType::kInt) {
    if (std::get<2>(right) == DataType::kInt) {
      std::get<2>(ret) = DataType::kInt;
      std::get<1>(ret) = sizeof(int64_t);
      std::get<0>(ret) = new char[sizeof(int64_t)];

      int64_t data = *reinterpret_cast<int64_t *>(std::get<0>(left)) / *reinterpret_cast<int64_t *>(std::get<0>(right));
      *reinterpret_cast<int64_t *>(std::get<0>(ret)) = data;
    }
    else if (std::get<2>(right) == DataType::kFloat) {
      std::get<2>(ret) = DataType::kFloat;
      std::get<1>(ret) = sizeof(double);
      std::get<0>(ret) = new char[sizeof(double)];

      double data = *reinterpret_cast<int64_t *>(std::get<0>(left)) / *reinterpret_cast<double *>(std::get<0>(right));
      *reinterpret_cast<double *>(std::get<0>(ret)) = data;
    }
  }
  else if (std::get<2>(left) == DataType::kFloat) {
    if (std::get<2>(right) == DataType::kInt) {
      std::get<2>(ret) = DataType::kFloat;
      std::get<1>(ret) = sizeof(double);
      std::get<0>(ret) = new char[sizeof(double)];

      double data = *reinterpret_cast<double *>(std::get<0>(left)) / *reinterpret_cast<int64_t *>(std::get<0>(right));
      *reinterpret_cast<double *>(std::get<0>(ret)) = data;
    }
    else if (std::get<2>(right) == DataType::kFloat) {
      std::get<2>(ret) = DataType::kFloat;
      std::get<1>(ret) = sizeof(double);
      std::get<0>(ret) = new char[sizeof(double)];

      double data = *reinterpret_cast<double *>(std::get<0>(left)) / *reinterpret_cast<double *>(std::get<0>(right));
      *reinterpret_cast<double *>(std::get<0>(ret)) = data;
    }
  }

  return ret;
}

std::tuple<void *, size_t, DataType> 
GreatThan(std::tuple<void *, size_t, DataType> left, std::tuple<void *, size_t, DataType> right) {
  std::tuple<void *, size_t, DataType> ret;
  
  if (std::get<2>(left) == DataType::kInt) {
    if (std::get<2>(right) == DataType::kInt) {
      std::get<2>(ret) = DataType::kInt;
      std::get<1>(ret) = sizeof(int64_t);
      std::get<0>(ret) = new char[sizeof(int64_t)];

      int64_t data = *reinterpret_cast<int64_t *>(std::get<0>(left)) > *reinterpret_cast<int64_t *>(std::get<0>(right));
      *reinterpret_cast<int64_t *>(std::get<0>(ret)) = data;
    }
    else if (std::get<2>(right) == DataType::kFloat) {
      std::get<2>(ret) = DataType::kInt;
      std::get<1>(ret) = sizeof(int64_t);
      std::get<0>(ret) = new char[sizeof(int64_t)];

      int64_t data = *reinterpret_cast<int64_t *>(std::get<0>(left)) > *reinterpret_cast<double *>(std::get<0>(right));
      *reinterpret_cast<int64_t *>(std::get<0>(ret)) = data;
    }
  }
  else if (std::get<2>(left) == DataType::kFloat) {
    if (std::get<2>(right) == DataType::kInt) {
      std::get<2>(ret) = DataType::kInt;
      std::get<1>(ret) = sizeof(int64_t);
      std::get<0>(ret) = new char[sizeof(int64_t)];

      int64_t data = *reinterpret_cast<double *>(std::get<0>(left)) > *reinterpret_cast<int64_t *>(std::get<0>(right));
      *reinterpret_cast<int64_t *>(std::get<0>(ret)) = data;
    }
    else if (std::get<2>(right) == DataType::kFloat) {
      std::get<2>(ret) = DataType::kInt;
      std::get<1>(ret) = sizeof(int64_t);
      std::get<0>(ret) = new char[sizeof(int64_t)];

      int64_t data = *reinterpret_cast<double *>(std::get<0>(left)) > *reinterpret_cast<double *>(std::get<0>(right));
      *reinterpret_cast<int64_t *>(std::get<0>(ret)) = data;
    }
  }

  return ret;
}

std::tuple<void *, size_t, DataType> 
LessThan(std::tuple<void *, size_t, DataType> left, std::tuple<void *, size_t, DataType> right) {
  std::tuple<void *, size_t, DataType> ret;
  
  if (std::get<2>(left) == DataType::kInt) {
    if (std::get<2>(right) == DataType::kInt) {
      std::get<2>(ret) = DataType::kInt;
      std::get<1>(ret) = sizeof(int64_t);
      std::get<0>(ret) = new char[sizeof(int64_t)];

      int64_t data = *reinterpret_cast<int64_t *>(std::get<0>(left)) < *reinterpret_cast<int64_t *>(std::get<0>(right));
      *reinterpret_cast<int64_t *>(std::get<0>(ret)) = data;
    }
    else if (std::get<2>(right) == DataType::kFloat) {
      std::get<2>(ret) = DataType::kInt;
      std::get<1>(ret) = sizeof(int64_t);
      std::get<0>(ret) = new char[sizeof(int64_t)];

      int64_t data = *reinterpret_cast<int64_t *>(std::get<0>(left)) < *reinterpret_cast<double *>(std::get<0>(right));
      *reinterpret_cast<int64_t *>(std::get<0>(ret)) = data;
    }
  }
  else if (std::get<2>(left) == DataType::kFloat) {
    if (std::get<2>(right) == DataType::kInt) {
      std::get<2>(ret) = DataType::kInt;
      std::get<1>(ret) = sizeof(int64_t);
      std::get<0>(ret) = new char[sizeof(int64_t)];

      int64_t data = *reinterpret_cast<double *>(std::get<0>(left)) < *reinterpret_cast<int64_t *>(std::get<0>(right));
      *reinterpret_cast<int64_t *>(std::get<0>(ret)) = data;
    }
    else if (std::get<2>(right) == DataType::kFloat) {
      std::get<2>(ret) = DataType::kInt;
      std::get<1>(ret) = sizeof(int64_t);
      std::get<0>(ret) = new char[sizeof(int64_t)];

      int64_t data = *reinterpret_cast<double *>(std::get<0>(left)) < *reinterpret_cast<double *>(std::get<0>(right));
      *reinterpret_cast<int64_t *>(std::get<0>(ret)) = data;
    }
  }

  return ret;
}

template <typename T, typename D>
bool In(const T &cont, const D &v) {
  return cont.find(v) != cont.end();
}

static bool In(const std::string &s, const std::string &c) {
  return s.find(c) != s.npos;
}

bool IsVarHead(char c);

bool IsVarBody(char c);

bool IsVar(const std::string &token);

bool IsInt(const std::string &token) {
  for (auto c : token) {
    if (c >= '0' && c <= '9') {}
    else {
      return false; 
    }
  }
  return true;
}

bool IsFloat(const std::string &token) {
  for (auto c : token) {
    if ((c >= '0' && c <= '9') || c == '.') {}
    else {
      return false; 
    }
  }

  if (In(token, std::string("."))) {
    return true;
  }
  else {
    return false;
  }
}

bool IsStr(const std::string &token) {
  if (token.size() >= 2 && token[0] == '"' && token[token.size() - 1] == '"') {
    return true;
  }
  else {
    return false;
  }
}

Token *ToToken(const std::string &name) {
  Token *token = nullptr;

  if (name == "=") {
    token = new AssignToken(name);
  }
  // else if (name == "if") {
  //   token = new IfToken(name);
  // }
  // else if (name == "<") {
  //   token = new LessThanToken(name);
  // }
  // else if (name == ":") {
  //   token = new ColonToken(name);
  // }
  else if (name == "print") {
    token = new PrintToken(name);
  }
  else if (name == "if") {
    token = new IfToken(name);
  }
  else if (name == "while") {
    token = new WhileToken(name);
  }
  // else if (name == "(") {
  //   token = new LeftP1Token(name);
  // }
  // else if (name == ")") {
  //   token = new RightP1Token(name);
  // }
  else if (IsVar(name)) {
    token = new VarToken(name);
  }
  else if (IsInt(name)) {
    token = new ConstIntToken(name);
  }
  else if (IsFloat(name)) {
    token = new ConstFloatToken(name);
  }
  else if (IsStr(name)) {
    token = new ConstStrToken(name);
  }
  else if (name == "+") {
    token = new AddToken(name);
  }
  else if (name == "*") {
    token = new MulToken(name);
  }
  else if (name == "-") {
    token = new SubToken(name);
  }
  else if (name == "/") {
    token = new DivToken(name);
  }
  else if (name == ">") {
    token = new GreatThanToken(name);
  }
  else if (name == "<") {
    token = new LessThanToken(name);
  }
  else {
    LOG(FATAL) << "Unexpected token " << name;
  }

  CHECK_NOTNULL(token);
  return token;
}

std::string Token::repr() {
  std::string s;

  switch (*data_type_) {
    case DataType::kFloat:
    {
      double *data = reinterpret_cast<double *>(*var_);
      s = std::to_string(*data);
      break;
    }

    case DataType::kInt:
    {
      int64_t *data = reinterpret_cast<int64_t *>(*var_);
      s = std::to_string(*data);
      break;
    }
    
    case DataType::kStr:
    {
      char *data = reinterpret_cast<char *>(*var_);
      s = std::string(data);
      break;
    }

    case DataType::kInvalid:
      s = "";
    
    default:
      LOG(FATAL) << "Unexpected data type " << static_cast<int>(*data_type_);
  }

  return std::move(s);
}

Token *Token::pop() {
  CHECK_GT(kids_.size(), 0);

  Token *kid = kids_[kids_.size() - 1];
  kids_.resize(kids_.size() - 1);

  kid->SetFather(nullptr);
  return kid;
}

void Token::DebugString(int deep, int level) {
  std::string prefix(deep * 2, ' ');
  std::string s = prefix + name_;

  if (level == 1) {
    s = s + " " + std::to_string(static_cast<int>(token_type_));
  }

  LOG(INFO) << s;

  for (auto *node : kids_) {
    node->DebugString(deep + 1, level);
  }
}

void GlobalToken::Run() {
  for (auto *kid : kids_) {
    kid->Run();
  }
}

void ListToken::Run() {
  for (auto *kid : kids_) {
    kid->Run();
    if (kid->token_type() == TokenType::kIf ||
        kid->token_type() == TokenType::kWhile) {
      break;
    }
  }
}

void VarToken::Prepare() {
  if (In(var_map, name_)) {
    var_ = std::get<0>(var_map[name_]);
    size_ = std::get<1>(var_map[name_]);
    data_type_ = std::get<2>(var_map[name_]);
  }
  else {
    var_ = new void*;
    *var_ = nullptr;
    size_ = new size_t(0);
    data_type_ = new DataType(DataType::kInvalid);
    var_map[name_] = std::tuple<void **, size_t *, DataType *>(var_, size_, data_type_);
  }
}

void AssignToken::Run() {
  size_t k = 0;
  for (; k < father_->kids().size(); ++k) {
    if (father_->kids()[k] == this) {
      break;
    }
  }

  Token *left = father_->kids()[k - 1];
  Token *right = father_->kids()[k + 1];

  right->Run();

  left->SetVar(right->var(), right->size(), right->data_type());
}

void ConstIntToken::Prepare() {
  var_ = new void*;
  *var_ = new char[sizeof(int64_t)];

  size_ = new size_t(sizeof(int64_t));
  data_type_ = new DataType(DataType::kInt);

  int64_t data = std::atoi(name_.c_str());
  SetVar(&data, *size_, *data_type_);
}

void ConstFloatToken::Prepare() {
  var_ = new void*;
  *var_ = new char[sizeof(double)];

  size_ = new size_t(sizeof(double));
  data_type_ = new DataType(DataType::kFloat);

  double data = std::atoi(name_.c_str());
  SetVar(&data, *size_, *data_type_);
}

void PrintToken::Run() {
  size_t k = 0;
  for (; k < father_->kids().size(); ++k) {
    if (father_->kids()[k] == this) {
      break;
    }
  }

  size_t k_pre = k;
  ++k_pre;
  for (; k_pre < father_->kids().size(); ++k_pre) {
    Token *next = father_->kids()[k_pre];
    Token *next_kid = next->kids()[0];
    next->SetVar(next_kid->var(), next_kid->size(), next_kid->data_type());
  }

  ++k;
  for (; k < father_->kids().size(); ++k) {
    Token *next = father_->kids()[k];
    next->Run();
    std::cout << next->repr() << " ";
  }
  std::cout << std::endl;

  // Token *right = father_->kids()[k + 1];

  // for (auto *kid : right->kids()) {
  //   kid->Run();
  //   std::cout << kid->repr();
  //   std::cout << " ";
  // }
  // std::cout << std::endl;
}

void ConstStrToken::Prepare() {
  var_ = new void*;
  *var_ = new char[name_.size() - 2 + 1];

  size_ = new size_t(name_.size() - 2 + 1);
  data_type_ = new DataType(DataType::kStr);

  SetVar(name_.c_str() + 1, *size_, *data_type_);
  reinterpret_cast<char *>(*var_)[name_.size() - 2] = '\0';
}

void AddToken::Run() {
  size_t k = 0;
  for (; k < father_->kids().size(); ++k) {
    if (father_->kids()[k] == this) {
      break;
    }
  }

  Token *left = father_->kids()[k - 1];
  Token *right = father_->kids()[k + 1];

  right->Run();

  std::tuple<void *, size_t, DataType> lelf_tp(left->var(), left->size(), left->data_type());
  std::tuple<void *, size_t, DataType> right_tp(right->var(), right->size(), right->data_type());
  auto ret = Add(lelf_tp, right_tp);

  father_->SetVar(std::get<0>(ret), std::get<1>(ret), std::get<2>(ret));
}

void MulToken::Run() {
  size_t k = 0;
  for (; k < father_->kids().size(); ++k) {
    if (father_->kids()[k] == this) {
      break;
    }
  }

  Token *left = father_->kids()[k - 1];
  Token *right = father_->kids()[k + 1];

  right->Run();

  std::tuple<void *, size_t, DataType> lelf_tp(left->var(), left->size(), left->data_type());
  std::tuple<void *, size_t, DataType> right_tp(right->var(), right->size(), right->data_type());
  auto ret = Mul(lelf_tp, right_tp);

  father_->SetVar(std::get<0>(ret), std::get<1>(ret), std::get<2>(ret));
}

void SubToken::Run() {
  size_t k = 0;
  for (; k < father_->kids().size(); ++k) {
    if (father_->kids()[k] == this) {
      break;
    }
  }

  Token *left = father_->kids()[k - 1];
  Token *right = father_->kids()[k + 1];

  right->Run();

  std::tuple<void *, size_t, DataType> lelf_tp(left->var(), left->size(), left->data_type());
  std::tuple<void *, size_t, DataType> right_tp(right->var(), right->size(), right->data_type());
  auto ret = Sub(lelf_tp, right_tp);

  father_->SetVar(std::get<0>(ret), std::get<1>(ret), std::get<2>(ret));
}

void DivToken::Run() {
  size_t k = 0;
  for (; k < father_->kids().size(); ++k) {
    if (father_->kids()[k] == this) {
      break;
    }
  }

  Token *left = father_->kids()[k - 1];
  Token *right = father_->kids()[k + 1];

  right->Run();

  std::tuple<void *, size_t, DataType> lelf_tp(left->var(), left->size(), left->data_type());
  std::tuple<void *, size_t, DataType> right_tp(right->var(), right->size(), right->data_type());
  auto ret = Div(lelf_tp, right_tp);

  father_->SetVar(std::get<0>(ret), std::get<1>(ret), std::get<2>(ret));
}

void GreatThanToken::Run() {
  size_t k = 0;
  for (; k < father_->kids().size(); ++k) {
    if (father_->kids()[k] == this) {
      break;
    }
  }

  Token *left = father_->kids()[k - 1];
  Token *right = father_->kids()[k + 1];

  right->Run();

  std::tuple<void *, size_t, DataType> lelf_tp(left->var(), left->size(), left->data_type());
  std::tuple<void *, size_t, DataType> right_tp(right->var(), right->size(), right->data_type());
  auto ret = GreatThan(lelf_tp, right_tp);

  father_->SetVar(std::get<0>(ret), std::get<1>(ret), std::get<2>(ret));
}

void LessThanToken::Run() {
  size_t k = 0;
  for (; k < father_->kids().size(); ++k) {
    if (father_->kids()[k] == this) {
      break;
    }
  }

  Token *left = father_->kids()[k - 1];
  Token *right = father_->kids()[k + 1];

  right->Run();

  std::tuple<void *, size_t, DataType> lelf_tp(left->var(), left->size(), left->data_type());
  std::tuple<void *, size_t, DataType> right_tp(right->var(), right->size(), right->data_type());
  auto ret = LessThan(lelf_tp, right_tp);

  father_->SetVar(std::get<0>(ret), std::get<1>(ret), std::get<2>(ret));
}

bool Yes(Token *token) {
  DataType dt = token->data_type();
  void *var = token->var();

  switch (dt) {
    case DataType::kFloat:
    {
      double *data = reinterpret_cast<double *>(var);
      return *data != 0;
      break;
    }

    case DataType::kInt:
    {
      int64_t *data = reinterpret_cast<int64_t *>(var);
      return *data != 0;
      break;
    }
    
    case DataType::kStr:
    {
      return token->size() != 1;
      break;
    }

    case DataType::kInvalid:
      LOG(FATAL) << "Unexpected data type " << static_cast<int>(dt);
      return false;

    default:
      LOG(FATAL) << "Unexpected data type " << static_cast<int>(dt);
      return false;
  }
};

void IfToken::Run() {
  size_t k = 0;
  for (; k < father_->kids().size(); ++k) {
    if (father_->kids()[k] == this) {
      break;
    }
  }

  Token *cond = father_->kids()[k + 1];
  cond->Run();

  Token *body = father_->kids()[k + 2];

  if (Yes(cond)) {
    body->Run();
  }
  else {
    if (father_->kids().size() >= k + 4) {
      father_->kids()[k + 3]->Run();
    }
  }
}

void WhileToken::Run() {
  size_t k = 0;
  for (; k < father_->kids().size(); ++k) {
    if (father_->kids()[k] == this) {
      break;
    }
  }

  Token *cond = father_->kids()[k + 1];
  Token *body = father_->kids()[k + 2];
  do {
    cond->Run();
    if (Yes(cond)) {
      body->Run();
    }
    else {
      break;
    }
  } while(true);
}

};

