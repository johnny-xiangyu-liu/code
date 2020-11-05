// parser.h
//
// last-edit-by: <>
//
// Description:
//
//////////////////////////////////////////////////////////////////////

#ifndef PARSER_H
#define PARSER_H



#include <memory>
#include <string>

#include "absl/strings/string_view.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "expression.h"
#include "lexer.h"
#include "token.h"

namespace xiangyuliu::parser {

class Parser {
 public:
  Parser(std::string program) : lexer_(program){}

  absl::StatusOr<std::unique_ptr<Expression>> Parse();

 private:

  Lexer lexer_;

};

}

#endif // PARSER_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
