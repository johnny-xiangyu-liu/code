#ifndef LEXER_H
#define LEXER_H

#include <memory>
#include <string>

#include "absl/strings/string_view.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "token.h"

namespace xiangyuliu::parser {

class Lexer {
 public:
  Lexer(std::string program) : program_(program), index_(0) {}

  absl::StatusOr<std::unique_ptr<Token>> ReadToken();

  bool HasMoreToken();
 private:
  bool CanContinue();
  void ConsumeWs();
  void Increment();
  char CurrentChar();
  absl::string_view GetContent(int start);

  absl::StatusOr<std::unique_ptr<Token>> MakeToken(TokenType type, int start);
  absl::StatusOr<std::unique_ptr<Token>> ReadString();
  absl::StatusOr<std::unique_ptr<Token>> ReadNumber();
  absl::StatusOr<std::unique_ptr<Token>> ReadOperator();
  absl::StatusOr<std::unique_ptr<Token>> ReadVariable();

  const std::string program_;
  int index_;
};

}


#endif
