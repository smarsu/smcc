// Copyright (c) 2020 smarsu. All Rights Reserved.

#pragma once
#include <map>

#include "core/lexer.h"
#include "core/ast.h"

static std::map<std::string, std::vector<std::unique_ptr<ExprAST>>> 
FuncTable;

