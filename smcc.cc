// Copyright (c) 2020 smarsu. All Rights Reserved.

#include <string>

#include "core/lexer.h"
#include "core/ast.h"
#include "core/parser.h"
#include "core/driver.h"

int main(int argv, char *args[]) {
  std::string path = args[1];

  auto lexer = Lexer(path);
  auto driver = Driver(lexer);

  driver.Parse();
  driver.Codegen();
}
