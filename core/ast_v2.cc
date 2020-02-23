// Copyright (c) 2020 smarsu. All Rights Reserved.

#include <fstream>
#include <set>
#include <string>
#include <stack>

#include "core/ast.h"
#include "core/logging.h"
#include "core/token.h"

namespace smcc {

template <typename T, typename D>
bool In(const T &cont, const D &v) {
  return cont.find(v) != cont.end();
}

static bool In(const std::string &s, const std::string &c) {
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

bool IsVarHead(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
};

bool IsVarBody(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_';
};

bool IsVar(const std::string &token) {
  if (token.size() == 0) return false;
  if (!IsVarHead(token[0])) return false;

  for (size_t t = 0; t < token.size(); ++t) {
    if (!IsVarBody(token[t])) return false;
  }

  return true;
}

std::string Read(const std::string &filename) {
  std::ifstream fb(filename);
  fb.seekg(0, fb.end);
  size_t size = fb.tellg();
  fb.seekg(0, fb.beg);

  char c[size + 2];
  c[size] = '\n';
  c[size + 1] = '\0';
  fb.read(c, size);

  std::string s(c);

  fb.close();
  return std::move(s);
}

std::string MoveComments(std::string &s) {
  size_t left = 0;
  size_t right = 0;

  bool find = false;
  for (size_t k = 0; k < s.size(); ++k) {
    char c = s[k];

    if (find) {
      if (c == '\n') {
        right = k + 1;

        s.erase(s.begin() + left, s.begin() + right);
        return MoveComments(s);
      }
    }
    else {
      if (c == '#') {
        left = k;
        find = true;
      }
    }
  }

  bool find2 = false;
  for (size_t k = 0; k < s.size() - 2; ++k) {
    if (find2) {
      if (s[k] == '"' && s[k + 1] == '"' && s[k + 2] == '"') {
        right = k + 2 + 1;

        s.erase(s.begin() + left, s.begin() + right);
        return MoveComments(s);
      }
    }
    else {
      if (s[k] == '"' && s[k + 1] == '"' && s[k + 2] == '"') {
        left = k;
        find2 = true;

        k = k + 2;
      }
    }
  }

  CHECK_EQ(find, false);
  CHECK_EQ(find2, false);

  return s;
}

std::vector<std::string> Split(const std::string s) {
  std::vector<std::string> tokens;
  std::string token;
  
  bool find_head = false;
  for (size_t k = 0; k < s.size(); ++k) {
    char c = s[k];

    if (find_head) {
      if (IsVarBody(c)) {
        token.push_back(c);
      }
      else {
        tokens.push_back(token);
        token.clear();

        token.push_back(c);
        tokens.push_back(token);
        token.clear();

        find_head = false;
      }
    }
    else {
      if (IsVarHead(c)) {
        token.push_back(c);

        find_head = true;
      }
      else {
        token.push_back(c);
        tokens.push_back(token);
        token.clear();
      }
    }
  }

  return std::move(tokens);
}

std::vector<std::string> Merge(std::vector<std::string> &tokens) {
  size_t left, right;
  bool find = false;

  std::string concat;
  for (size_t t = 0; t < tokens.size(); ++t) {
    std::string token = tokens[t];

    if (find) {
      if (token == " ") {
        concat += token;
      }
      else {
        right = t;
        tokens.erase(tokens.begin() + left, tokens.begin() + right);

        tokens.insert(tokens.begin() + left, concat);
        return Merge(tokens);
      }
    }
    else {
      if (token == "\n" && t + 1 < tokens.size() && tokens[t + 1] == " ") {
        concat = "\n";
        left = t;

        find = true;
      }
    }
  }

  {
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
  }

  {
    bool find = false;
    std::vector<std::string> tks;
    std::string integer;
    for (size_t t = 0; t < tokens.size(); ++t) {
      std::string token = tokens[t];

      if (find) {
        if (token.size() == 1 && ((token[0] >= '0' && token[0] <= '9') || token[0] == '.')) {
          integer += token;
        }
        else {
          tks.push_back(integer);
          tks.push_back(token);

          integer.clear();
          find = false;
        }
      }
      else {
        if (token.size() == 1 && ((token[0] >= '0' && token[0] <= '9') || token[0] == '.')) {
          integer = token;
          find = true;
        }
        else {
          tks.push_back(token);
        }
      }
    }

    if (find == true) {
      tks.push_back(integer);
    }

    tokens = tks;
  }

  return tokens;
}

std::vector<std::string> Remove(std::vector<std::string> &tokens) {
  static std::set<std::string> rm = {
    " ",
  };
  std::vector<std::string> tokens_kept;
  for (auto &token : tokens) {
    if (!In(rm, token)) {
      tokens_kept.emplace_back(token);
    }
  }
  return std::move(tokens_kept);
}

Token *Tree(const std::vector<std::string> &tokens) {
  Token *tree = new GlobalToken("global");

  std::stack<Token *> stk;
  stk.push(tree);

  Token *node = new ListToken("list");
  stk.push(node);

  std::vector<int> black_level = {-1, 0};
  for (size_t t = 0; t < tokens.size(); ++t) {
    std::string token = tokens[t];

    if (In(token, std::string("\n"))) {
      int b_level = token.size() - 1;

      if (b_level > black_level[black_level.size() - 1]) {
        stk.push(new ListToken("list"));

        black_level.push_back(b_level);
      }
      else if (b_level == black_level[black_level.size() - 1]) {
        auto last = pop(stk);
        stk.top()->push(last);

        Token *node = new ListToken("list");
        stk.push(node);
      }
      else {
        CHECK(In(black_level, b_level));

        while (b_level <= black_level[black_level.size() - 1]) {
          auto last = pop(stk);
          stk.top()->push(last);

          black_level.resize(black_level.size() - 1); 
        }
        black_level.push_back(b_level);

        Token *node = new ListToken("list");
        stk.push(node);
      }
    } 
    else if (token == "(") {      
      // Token *last = stk.top()->pop();
      // stk.push(last);
      stk.push(new ListToken("list"));
    }
    else if (token == ")") {
      Token *last = pop(stk);
      stk.top()->push(last);
    }
    else if (token == "if") {
      stk.top()->push(ToToken(token));
      stk.push(new ListToken("list"));
    }
    else if (token == ":") {
      Token *cond = pop(stk);
      Token *iftoken = stk.top()->pop();
      stk.push(iftoken);
      stk.top()->push(cond);
    }
    else {
      stk.top()->push(ToToken(token));
    }
  }

  return tree;
}

Token *Tree_V2(const std::vector<std::string> &tokens, size_t pos = 0) {
  static std::set<std::string> funcs = {
    "print"
  };

  std::vector<int> black_level = {-1, 0};

  std::stack<Token *> stk;

  Token *tree = new GlobalToken("global");
  stk.push(tree);
  stk.push(new ListToken("list"));

  for (size_t t = pos; t < tokens.size(); ++t) {
    std::string token = tokens[t];

    if (In(token, std::string("\n"))) {
      int b_level = token.size() - 1;
      if (b_level > black_level[black_level.size() - 1]) {
        black_level.push_back(b_level);
      }

      Token *last = pop(stk);
      stk.top()->push(last);

      if (b_level < black_level[black_level.size() - 1]) {
        while (b_level <= black_level[black_level.size() - 1]) {
          Token *last = pop(stk);
          stk.top()->push(last);
        
          black_level.resize(black_level.size() - 1);
        }
      }

      stk.push(new ListToken("list"));
    }
    else if (token == "(") {
      stk.push(new ListToken("list"));
    }
    else if (token == ")") {
      Token *last = pop(stk);
      stk.top()->push(last);
    }
    else if (token == ",") {
      Token *last = pop(stk);
      stk.top()->push(last);

      stk.push(new ListToken("list"));
    }
    else if (token == ":") {
      stk.push(new ListToken("list"));
      stk.push(new ListToken("list"));     
    }
    else {
      stk.top()->push(ToToken(token));
    }
  }

  while (stk.size() > 1) {
    Token *last = pop(stk);
    stk.top()->push(last);
  }

  return tree;
}

void reorder(Token *node, const std::set<std::string> &ops) {
  for (size_t k = 0; k < node->kids().size(); ++k) {
    Token *sub = node->kids()[k];
    if (In(ops, sub->name()) && k != 1) {
      Token *lst = new ListToken("list");
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

Token *Reorder(Token *node) {
  std::vector<std::set<std::string>> ops_list = {
    {"*", "/"},
    {"+", "-"},
    {">", "<", "==", ">=", "<=", "!="}
    // {"="}
  };

  for (auto &ops : ops_list) {
    reorder(node, ops);
  }
  
  return node;
}

void AST::ParseV2(const std::string &filename, bool S) {
  std::string s = Read(filename);
  LOG(INFO) << s;

  s = MoveComments(s);
  LOG(INFO) << s;

  auto tokens = Split(s);

  tokens = Merge(tokens);

  tokens = Remove(tokens);

  for (auto token : tokens) {
    LOG(INFO) << token;
  }

  // global_ = Tree(tokens);
  global_ = Tree_V2(tokens, 0);

  global_ = Reorder(global_);

  global_->DebugString(1, 1);
}

}  // namespace smcc
