#include "lexer.h"

#include <cctype>
#include <memory>
#include <iostream>

#include "absl/base/macros.h"
#include "absl/strings/string_view.h"
#include "token.h"

namespace xiangyuliu::parser {
namespace {

  bool IsNumericOperator(char ch) {
    switch(ch) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
      return true;
    default:
      return false;
    }
  }
}  // namespace

  void Lexer::ConsumeWs(){
    while(CanContinue()) {
      if (!isspace(CurrentChar())) break;
      Increment();
    }
  }

  bool Lexer::HasMoreToken() {
    ConsumeWs();
    return CanContinue();
  }

  bool Lexer::CanContinue() {
    return index_ < program_.size();
  }

  void Lexer::Increment() {
    ++index_;
  }

  char Lexer::CurrentChar() {
    return program_.at(index_);
  }

  absl::string_view Lexer::GetContent(int start) {
    ABSL_ASSERT(start < index_);
    absl::string_view temp = program_;
    return temp.substr(start, index_ - start);
  }

  absl::StatusOr<std::unique_ptr<Token>> Lexer::MakeToken(TokenType type, int start) {
    return std::make_unique<Token>(type, GetContent(start));
  }

  absl::StatusOr<std::unique_ptr<Token>> Lexer::ReadString() {
    if (CurrentChar() != '"') {
      return absl::FailedPreconditionError("ReadString should start with \".");
    }
    bool found_ending_quote = false;
    bool escaped = false;
    Increment(); // Consume the '"'
    int start = index_;
    while (CanContinue()) {
      char ch = CurrentChar();
      if (ch == '\\') {
        escaped = !escaped;
      } else {
        escaped = false;
      }

      if (ch == '"' && !escaped) {
        found_ending_quote = true;
        break;
      }
      Increment();
    }
    if (!found_ending_quote) {
      return absl::FailedPreconditionError("Failed to find the closing quote");
    }
    auto token = MakeToken(TokenType::kString, start);
    Increment(); // Consume the closing quote.
    return token;
  }

  absl::StatusOr<std::unique_ptr<Token>> Lexer::ReadOperator() {
    int start = index_;
    Increment();
    return MakeToken(TokenType::kOperator, start);
  }

  absl::StatusOr<std::unique_ptr<Token>> Lexer::ReadNumber() {
    int start = index_;
    bool has_dot = false;
    while (CanContinue()) {
      char ch = CurrentChar();
      if (!isdigit(ch) && ch != '.') {
        break;
      }

      if (ch == '.') {
        if (has_dot) {
          return absl::FailedPreconditionError("Malformed number. a '.' has occurred already.");
        }
        has_dot = !has_dot;
      }
      Increment();
    }
    return MakeToken(TokenType::kNumber, start);
  }

  absl::StatusOr<std::unique_ptr<Token>> Lexer::ReadToken() {
    ConsumeWs();
    if (!CanContinue()) {
      return absl::FailedPreconditionError("Cannot Continue");
    }
    char ch = CurrentChar();

    int start = index_;
    if (ch == '"') return ReadString();
    if (IsNumericOperator(ch)) return ReadOperator();
    if (isdigit(ch)) return ReadNumber();

    if (start >= index_) {
      return absl::FailedPreconditionError("start < index_");
    }
    return nullptr;
  }

}
