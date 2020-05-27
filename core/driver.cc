// // Copyright (c) 2020 smarsu. All Rights Reserved.

#include "core/ast.h"
#include "core/driver.h"
#include "core/lexer.h"
#include "core/parser.h"

void MainLoop(Lexer &lexer) {
  lexer.nexttok();
  while (1) {
    switch (lexer.curtok()) {
      case '#':
        ParseInclude(lexer);
    }
  }
}

// void HandleTopLevelExpression(FILE *fb) {
//   if (ParseTopLevelExpr(fb)) {
//     fprintf(stderr, "Parse a top-level expr\n");
//   }
//   else {
//     getNextToken(fb);
//   }
// }

// void MainLoop(FILE *fb) {
//   while (1) { 
//     switch (getcurtok()) {
//       case tok_eof:
//         return;
      
//       case ';':
//         getNextToken(fb);
//         break;

//       default:
//         HandleTopLevelExpression(fb);
//     }
//   }
// }
