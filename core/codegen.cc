// Copyright (c) 2020 smarsu. All Rights Reserved.

#include <stack>
#include <map>

#include "core/codegen.h"
#include "core/node.h"
#include "core/logging.h"

namespace smcc {

template <typename T, typename D>
bool In(const T &cont, const D &v) {
  return cont.find(v) != cont.end();
}

bool In(const std::string &s, const std::string &c) {
  return s.find(c) != s.npos;
}

template <typename T>
bool In(const std::vector<T> &list, const T &v) {
  for (auto &value : list) {
    if (value == v) {
      return true;
    }
  }
  return false;
}

template <typename T>
T pop(std::stack<T> &cont) {
  T v = cont.top();
  cont.pop();
  return std::move(v);
}

void Assign(double *a, const double *b) { *a = *b; }

void Add(double *c, const double *a, const double *b) { *c = *a + *b; }

void Sub(double *c, const double *a, const double *b) { *c = *a - *b; }

void Mul(double *c, const double *a, const double *b) { *c = *a * *b; }

void Div(double *c, const double *a, const double *b) { *c = *a / *b; }

void Print(double *a) { printf("%f\n", *a); }

static std::set<std::string> operators = {
  "=",
  "+",
  "-",
  "*",
  "/",
  "print"
};

static std::set<std::string> ignores = {
  "=",
  "+",
  "-",
  "*",
  "/",
  "print",
  "list"
};

// template <typename T>
// T pop(std::stack<T> &cont) {
//   T v = cont.top();
//   cont.pop();
//   return std::move(v);
// }

// template <typename T, typename D>
// bool In(const T &cont, const D &v) {
//   return cont.find(v) != cont.end();
// }

void Run(Node *node) {
  for (Node *sub : node->kids()) {
    if (sub->kids().size() > 0) {
      Run(sub);
    }
  }

  std::stack<Node *> vars;
  for (size_t n = 0; n < node->kids().size(); ++n) {
    Node *sub = node->kids()[n];

    if (In(operators, sub->name())) {
      auto op = sub->name();
      if (op == "=") {
        Node *v = pop(vars);
        Assign(v->value<double>(), node->kids()[++n]->value<double>());
      }
      else if (op == "+") {
        Node *v = pop(vars);
        Add(v->father()->value<double>(), v->value<double>(), node->kids()[++n]->value<double>());
      }
      else if (op == "-") {
        Node *v = pop(vars);
        Sub(v->father()->value<double>(), v->value<double>(), node->kids()[++n]->value<double>());
      }
      else if (op == "*") {
        Node *v = pop(vars);
        Mul(v->father()->value<double>(), v->value<double>(), node->kids()[++n]->value<double>());
      }
      else if (op == "/") {
        Node *v = pop(vars);
        Div(v->father()->value<double>(), v->value<double>(), node->kids()[++n]->value<double>());
      }
      else if (op == "print") {
        for (size_t k = n + 1; k < node->kids().size(); ++k) {
          Node *sub = node->kids()[k];
          if (sub->type() == Type::kInt) {
            std::cout << *sub->value<int64_t>() << " ";
          }
          else if (sub->type() == Type::kString) {
            std::cout << sub->value<char>() << " ";
          }
          else if (sub->type() == Type::kFloat) {
            std::cout << *sub->value<double>() << " ";
          }
        }
        std::cout << std::endl;

        n = node->kids().size();
        // Print(node->kids()[++n]->value<int>());
      }
      else {
        abort();
      }
      
    }
    else {
      vars.push(sub);
    }
  }

  // node->DebugString();
  // std::cout << "------" << std::endl;
}

bool Interger(Node *node) {
  std::string name = node->name();
  for (char c : name) {
    if ((c >= '0' && c <= '9') || c == '.') {}
    else {
      return false;
    }
  }

  *(node->value<double>()) = std::atof(name.c_str());
  node->SetType(Type::kFloat);
  return true;
}

bool Str(Node *node) {
  std::string name = node->name();
  if (name[0] == '"' && name[name.size() - 1] == '"') {
    free(node->value<void>());

    void *ptr = malloc(name.size() - 2 + 1);
    memcpy(ptr, name.c_str() + 1, name.size() - 1);
    reinterpret_cast<char *>(ptr)[name.size() - 2] = '\0';

    node->SetValue(ptr);
    node->SetType(Type::kString);
    return true;
  }
  else {
    return false;
  }
}

void SetValue(Node *tree, std::map<std::string, void *> &values) {
  for (Node *sub : tree->kids()) {
    if (!In(ignores, sub->name())) {
      if (Interger(sub)) {}
      if (Str(sub)) {}
      else {
        if (In(values, sub->name())) {
          sub->SetValue(values[sub->name()]);
        }
        else {
          values[sub->name()] = sub->value<void>();
        }
      }
    }
    SetValue(sub, values);
  }
}

void CodeGen::VM() {
  Node *tree = ast_.tree();

  std::map<std::string, void *> values;
  SetValue(tree, values);

  // tree->DebugString();

  for (Node *node : tree->kids()) {
    Run(node);
  }
}

void Prepare(Token *token) {
  for (auto *kid : token->kids()) {
    kid->Prepare();
    Prepare(kid);
  }
  token->Prepare();
}

void Run(Token *token) {
  token->Run();
}

void CodeGen::VM2() {
  Token *tree = ast_.tree_v2();

  Prepare(tree);
  Run(tree);
}

}  // namespace smcc
