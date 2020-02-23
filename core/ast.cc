// Copyright (c) 2020 smarsu. All Rights Reserved.

#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <set>
#include <utility>

#include "core/ast.h"
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

std::string Read(const std::string &filename) {
  std::ifstream fb(filename);
  fb.seekg(0, fb.end);
  size_t size = fb.tellg();
  fb.seekg(0, fb.beg);

  char c[size + 1];
  c[size] = '\0';
  fb.read(c, size);

  std::string s(c);

  fb.close();
  return std::move(s);
}

std::string ClearComments(const std::string &s) {
  return s;
}

std::vector<std::string> Split(const std::string &s) {
  static std::set<char> kps = {
    ' ',
    '\n',
    '\t',
    '=',
    '+',
    '-',
    '*',
    '/',
    '(',
    ')',
    ',',
    '"'
  };
  static std::set<char> rm = {
  };

  std::vector<std::string> tokens;
  std::string token;

  for (char c : s) {
    if (In(kps, c)) {
      if (token.size() > 0) {
        tokens.emplace_back(token);
        token.clear();
      }
      if (!In(rm, c)) {
        std::string token;
        token.push_back(c);
        tokens.emplace_back(token);
      }
    }
    else {
      token.push_back(c);
    }
  }

  if (token.size() > 0) {
    tokens.emplace_back(token);
  }

  return std::move(tokens);
}

std::vector<std::string> Merge(std::vector<std::string> &tokens) {
  bool tag = false;
  size_t l = -1;
  size_t r = -1;
  std::string mg;
  for (size_t t = 0; t < tokens.size(); ++t) {
    std::string tk = tokens[t];

    if (tag) {
      mg += tk;

      if (tk == "\"") {
        tag = false;

        r = t + 1;
        tokens.erase(tokens.begin() + l, tokens.begin() + r);
        tokens.insert(tokens.begin() + l, mg);
        return Merge(tokens);
      }
    }
    else {
      if (tk == "\"") {
        tag = true;
        mg = tk;
        l = t;
      }
    }
  }

  CHECK_EQ(tag, false);
  return tokens;
}

std::vector<std::string> Remove(const std::vector<std::string> &tokens) {
  static std::set<std::string> rm = {
    " ",
    "\t",
    ","
  };
  std::vector<std::string> tokens_kept;
  for (auto &token : tokens) {
    if (!In(rm, token)) {
      tokens_kept.emplace_back(token);
    }
  }
  return std::move(tokens_kept);
}

void reorder(Node *node, const std::set<std::string> &ops) {
  for (size_t k = 0; k < node->kids().size(); ++k) {
    Node *sub = node->kids()[k];
    if (In(ops, sub->name()) && k != 1) {
      Node *lst = new Node("list");
      lst->push(node->kids()[k - 1]);
      lst->push(node->kids()[k]);
      lst->push(node->kids()[k + 1]);

      node->kids().erase(node->kids().begin() + k - 1, node->kids().begin() + k + 2);
      node->kids().insert(node->kids().begin() + k - 1, lst);
      reorder(node, ops);
      return;
    }

    reorder(sub, ops);
  }
}

Node *Reorder(Node *node) {
  std::vector<std::set<std::string>> ops_list = {
    {"*", "/"},
    {"+", "-"},
    {"="}
  };

  for (auto &ops : ops_list) {
    reorder(node, ops);
  }
  
  return node;
}


Node *Tree(const std::vector<std::string> &tokens, Node *tree) {
  static std::set<std::string> lt = {
    "("
  };

  static std::set<std::string> rt = {
    ")"
  };

  std::stack<Node *> stk;
  Node *global = tree;
  stk.push(global);

  Node *node = new Node("list");
  stk.push(node);
  for (size_t t = 0; t < tokens.size(); ++t) {
    std::string name = tokens[t];
    Node *node = new Node(name);

    if (In(name, std::string("\n"))) {
      Node *last = pop(stk);
      stk.top()->push(last);

      Node *node = new Node("list");
      stk.push(node);
    }
    else if (In(lt, name)) {
      // Node *node = new Node("list");
      // stk.push(node);

      Node *node = new Node("list");

      if (stk.top()->kids().size() > 0) {
        Node *last = stk.top()->pop();
        node->push(last);
      }

      stk.push(node);
    }
    else if (In(rt, name)) {
      Node *last = pop(stk);
      stk.top()->push(last);
    }
    else {
      stk.top()->push(node);
    }
  }

  return global;
}

void AST::Parse(const std::string &filename, bool S) {
  std::string s = Read(filename);

  s = ClearComments(s);

  std::vector<std::string> tokens = Split(s);

  tokens = Merge(tokens);

  tokens = Remove(tokens);

  Tree(tokens, tree_);

  Reorder(tree_);

  if (S) tree_->DebugString();
}

}  // namespace smcc