// Copyright (c) 2020 smarsu. All Rights Reserved.

#include <map>
#include <memory>

#include "core/parser.h"
#include "core/lexer.h"
#include "core/ast.h"

std::unique_ptr<ExprAST> LogError(const char *Str) {
  fprintf(stderr, "LogError: %s\n", Str);
  return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

std::unique_ptr<FunctionAST> LogErrorF(const char *Str) {
  LogError(Str);
  return nullptr;
}

namespace std {

template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params) {
  return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

}

std::unique_ptr<ExprAST> ParseStrExpr(Lexer &lexer) {
  auto result = std::make_unique<StrExprAST>(lexer.identifier_str());
  lexer.nexttok();
  return std::move(result);
}

std::unique_ptr<ExprAST> ParseParenExpr(Lexer &lexer) {
  lexer.nexttok();  // eaten '('
  auto expr = ParseExpression(lexer);
  if (!expr) return nullptr;

  if (lexer.curtok() != ')') return LogError("Expected ')");
  lexer.nexttok();  // eaten ')'
  return expr;
}

std::unique_ptr<ExprAST> ParseIdentifierExpr(Lexer &lexer) {
  std::string id_name = lexer.identifier_str();

  lexer.nexttok();  // eaten 'name'

  if (lexer.curtok() != '(') {
    return std::make_unique<VariableExprAST>("", id_name);
  }

  lexer.nexttok();  // eaten '('
  std::vector<std::unique_ptr<ExprAST>> args;
  if (lexer.curtok() != ')') {
    while (1) {
      if (auto arg = ParseExpression(lexer)) 
        args.push_back(std::move(arg));
      else
        return nullptr;

      if (lexer.curtok() == ')')
        break;

      if (lexer.curtok() != ',')
        return LogError("Expected ')' or ',' in argument list");

      lexer.nexttok();
    }
  }

  lexer.nexttok();  // eaten ')'
  return std::make_unique<CallExprAST>(id_name, std::move(args));
}

std::unique_ptr<ExprAST> ParseInclude(Lexer &lexer) {
  if (lexer.curtok() != '#') return LogError("Expected #include");
  lexer.nexttok();  // eaten '#'
  if (lexer.curtok() != tok_include) return LogError("Expected #include");
  lexer.nexttok();  // eaten 'include'

  std::string path = lexer.identifier_str();
  lexer.nexttok();
  return std::make_unique<IncludeExprAST>(path);
}

std::unique_ptr<ExprAST> ParsePrototype(Lexer &lexer) {
  if (lexer.curtok() != tok_type)
    return LogError("Expected type in prototype");
  
  std::string type = lexer.identifier_str();
  lexer.nexttok();

  if (lexer.curtok() != tok_identifier)
    return LogError("Expected function name in prototype");

  std::string fn_name = lexer.identifier_str();
  lexer.nexttok();

  if (lexer.curtok() != '(') {
    return std::make_unique<VariableExprAST>(type, fn_name);
  }

  lexer.nexttok();

  std::vector<std::unique_ptr<ExprAST>> args;

  while (auto expr = ParseVariable(lexer))
    args.push_back(std::move(expr));

  if (lexer.curtok() != ')')
    return LogError("Expected ')' in prototype");

  lexer.nexttok();
  return std::make_unique<PrototypeAST>(type, fn_name, std::move(args));
}

std::unique_ptr<VariableExprAST> ParseVariable(Lexer &lexer) {
  return nullptr;
}

std::unique_ptr<ExprAST> ParseDefinition(Lexer &lexer) {
  auto proto = ParsePrototype(lexer);
  if (!proto) return nullptr;

  if (lexer.curtok() != '{')
    return proto;

  lexer.nexttok();
  std::vector<std::unique_ptr<ExprAST>> exprs;
  if (lexer.curtok() != '}') {
    while (1) {
      if (auto arg = ParseExpression(lexer)) 
        exprs.push_back(std::move(arg));
      else
        return nullptr;

      if (lexer.curtok() != ';')
        return LogError("Expected '}' or ';' in expr list");

      lexer.nexttok();

      if (lexer.curtok() == '}')
        break;
    }
  }
  
  lexer.nexttok();
  return std::make_unique<FunctionAST>(std::move(proto), std::move(exprs));
}

std::unique_ptr<ExprAST> ParseExpression(Lexer &lexer) {
  auto LHS = ParsePrimary(lexer);
  if (!LHS)
    return nullptr;

  return ParseBinOpRHS(lexer, 0, std::move(LHS));
}

std::unique_ptr<ExprAST> ParseReturnExpr(Lexer &lexer) {
  if (lexer.curtok() != tok_return)
    LogError("Expected return in ParseReturnExpr");

  lexer.nexttok();  // eat return

  auto expr = ParseExpression(lexer);
  return std::make_unique<ReturnExprAST>(std::move(expr));
}

std::unique_ptr<NumberExprAST> ParseNumberExpr(Lexer &lexer) {
  auto result = std::make_unique<NumberExprAST>(lexer.numval());
  lexer.nexttok();
  return std::move(result);
}

std::unique_ptr<ExprAST> ParsePrimary(Lexer &lexer) {
  switch (lexer.curtok()) {
    case tok_identifier:
      return ParseIdentifierExpr(lexer);

    case tok_str:
      return ParseStrExpr(lexer);

    case '(':
      return ParseParenExpr(lexer);

    case tok_return:
      return ParseReturnExpr(lexer);

    case tok_number:
      return ParseNumberExpr(lexer);

    default:
      return LogError("Unknown token when expectin an expression");
  }
}

static std::map<char, int> BinopPrecedence;

int GetTokPrecedence(Lexer &lexer) {
  static bool init = false;
  if (!init) {
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;
    init = true;
  }

  if (!isascii(lexer.curtok()))
    return -1;

  int tokprec = BinopPrecedence[lexer.curtok()];
  if (tokprec <= 0) return -1;
  return tokprec;
}

std::unique_ptr<ExprAST> ParseBinOpRHS(Lexer &lexer, int exprprec, std::unique_ptr<ExprAST> LHS) {
  while (1) {
    int tokprec = GetTokPrecedence(lexer);

    if (tokprec < exprprec)
      return LHS;

    int bin_op = lexer.curtok();
    lexer.nexttok();

    auto RHS = ParsePrimary(lexer);
    if (!RHS)
      return nullptr;

    int nextprec = GetTokPrecedence(lexer);
    if (tokprec < nextprec) {
      RHS = ParseBinOpRHS(lexer, tokprec + 1, std::move(RHS));
      if (!RHS)
        return nullptr;
    }

    LHS = std::make_unique<BinaryExprAST>(bin_op, std::move(LHS), std::move(RHS));
  }
} 
