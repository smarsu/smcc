// Copyright (c) 2020 smarsu. All Rights Reserved.

#include <iostream>
#include <map>
#include <string>

#include "core/api.h"

template <typename T, typename D>
bool In(const T &cont, const D &v) {
  return cont.find(v) != cont.end();
}

std::map<std::string, std::string> ParseInput(int argv, char *args[]) {
  std::vector<std::string> inputs(argv - 1);
  for (int t = 1; t < argv; ++t) {
    inputs[t - 1] = args[t];
  }

  std::map<std::string, std::string> arg;

  for (auto &s : inputs) {
    if (s == "-S") {
      arg["S"] = "true";
    }
    else {
      if (In(arg, "input")) {
        std::cout << "Input Error" << std::endl;
        exit(-1);
      }
      else {
        arg["input"] = s;
      }
    }
  }

  return arg;
}

int main(int argv, char *args[]) {
  auto arg = ParseInput(argv, args);

  smcc::AST ast(arg["input"], In(arg, "S"));

  smcc::CodeGen codegen(ast);

  return 0;
}
