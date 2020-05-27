// Copyright (c) 2020 smarsu. All Rights Reserved.

#pragma once
#include <cassert>

#include "core/parser.h"
#include "core/lexer.h"
#include "core/codegen.h"

class Driver {
 public:
  Driver(Lexer &lexer)
    : lexer_(lexer) {}

  void Parse() {
    lexer_.nexttok();
    while (1) {
      switch (lexer_.curtok()) {
        case tok_eof:
          return;
        
        case '#':
          asts.push_back(std::move(ParseInclude(lexer_)));
          break;

        case tok_type:
          asts.push_back(std::move(ParseDefinition(lexer_)));
          break;

        case ';':
          break;

        default:
          fprintf(stderr, "%c\n", lexer_.curtok());
          fprintf(stderr, "%d\n", lexer_.curtok());
          fprintf(stderr, "%s\n", lexer_.identifier_str().c_str());
          assert(false);
          return;
      }
    } 
  }

  void Codegen() {
    for (auto &ast : asts) {
      ast->codegen();
    }
    main = std::unique_ptr<CallExprAST>(new CallExprAST("main", std::vector<std::unique_ptr<ExprAST>>()));
    main->codegen();
  }

 private:
  std::vector<std::unique_ptr<ExprAST>> asts;
  std::unique_ptr<ExprAST> main;
  Lexer lexer_;
};

