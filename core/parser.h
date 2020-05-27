// // Copyright (c) 2020 smarsu. All Rights Reserved.

#pragma once
#include <memory>

#include "core/lexer.h"
#include "core/ast.h"

std::unique_ptr<ExprAST> ParseStrExpr(Lexer &lexer);

std::unique_ptr<ExprAST> ParseParenExpr(Lexer &lexer);

std::unique_ptr<ExprAST> ParseIdentifierExpr(Lexer &lexer);

std::unique_ptr<ExprAST> ParsePrimary(Lexer &lexer);

std::unique_ptr<ExprAST> ParseExpression(Lexer &lexer);

std::unique_ptr<ExprAST> ParseBinOpRHS(Lexer &lexer, int ExprPrec, std::unique_ptr<ExprAST> LHS);

std::unique_ptr<ExprAST> ParsePrototype(Lexer &lexer);

std::unique_ptr<ExprAST> ParseDefinition(Lexer &lexer);

std::unique_ptr<PrototypeAST> ParseExtern(Lexer &lexer);

std::unique_ptr<FunctionAST> ParseTopLevelExpr(Lexer &lexer);

std::unique_ptr<VariableExprAST> ParseVariable(Lexer &lexer);

std::unique_ptr<ExprAST> ParseInclude(Lexer &lexer);
