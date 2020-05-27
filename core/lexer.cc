// Copyright (c) 2020 smarsu. All Rights Reserved.

#include <string>
#include <cassert>

#include "core/lexer.h"

int Lexer::gettok() {
  while (isspace(last_char_)) last_char_ = fgetc(fb_);

  if (isalpha(last_char_)) {
    identifier_str_ = last_char_;
    while (isalnum(last_char_ = fgetc(fb_))) identifier_str_ += last_char_;

    if (identifier_str_ == "include") 
      return tok_include;

    if (identifier_str_ == "int")
      return tok_type;

    if (identifier_str_ == "double")
      return tok_type;

    if (identifier_str_ == "return")
      return tok_return;

    return tok_identifier;
  }

  if (isdigit(last_char_) || last_char_ == '.') {   // Number: [0-9.]+
    std::string num_str;
    do {
      num_str += last_char_;
      last_char_ = fgetc(fb_);
    } while (isdigit(last_char_) || last_char_ == '.');

    num_val_ = strtod(num_str.c_str(), 0);
    return tok_number;
  }

  if (last_char_ == '"') {
    identifier_str_ = "";
    while ((last_char_ = fgetc(fb_)) != '"') {
      identifier_str_ += last_char_;
      assert(last_char_ != EOF && "Not found \" after \"");
    }

    identifier_str_ += '\0';

    last_char_ = fgetc(fb_);
    return tok_str;
  }

  if (last_char_ == '/') {
    last_char_ = fgetc(fb_);
    assert(last_char_ == '/' && "Not found / after /");

    do 
      last_char_ = fgetc(fb_);
    while (last_char_ != EOF && last_char_ != '\n' && last_char_ != '\r');

    if (last_char_ != EOF) return gettok();
  }

  if (last_char_ == EOF) return tok_eof;

  int this_char = last_char_;
  last_char_ = fgetc(fb_);
  return this_char;
}
