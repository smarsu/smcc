// Copyright (c) 2020 smarsu. All Rights Reserved.

#pragma once
#include <string>

enum Token {
  tok_eof = -1,
  
  tok_identifier = -2,
  tok_str = -3,
  tok_include = -4,
  tok_type = -5,
  tok_return = -6,
  tok_number = -7,
};

class Lexer {
 public:
  Lexer(const std::string &path) 
    : fb_(fopen(path.c_str(), "r")) {}

  int nexttok() { 
    return curtok_ = gettok(); 
  }

  const std::string &identifier_str() const { return identifier_str_; }

  int curtok() { return curtok_; }

  double numval() { return num_val_; }

 private:
  int gettok();

 private:
  FILE *fb_{nullptr};
  std::string identifier_str_;
  int curtok_;
  int last_char_ = ' ';
  double num_val_;
};
