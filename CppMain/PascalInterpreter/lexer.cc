#include "lexer.h"

#include <cctype>
#include <iostream>
#include <memory>
#include <regex>

#include "absl/base/macros.h"
#include "absl/strings/string_view.h"
#include "absl/strings/substitute.h"
#include "token.h"
#include "util.h"

namespace xiangyuliu::parser {
namespace {

  static const auto*  kVariableRegex = new std::regex("[a-zA-Z0-9_]*");

  template <typename... T>
  void Debug(const T&...  message) {
    int unused[] = {(std::cout << message, 0)...};

    std::cout << std::endl;
  }

  bool Match(absl::string_view content, const std::regex& regex_expr) {
    return std::regex_match(content.begin(), content.end(), regex_expr);
  }

  bool Match(char ch, const std::regex& regex_expr) {
    return Match(absl::string_view(&ch, 1), regex_expr);
  }

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
      if (ch == '"' && !escaped) {
        found_ending_quote = true;
        break;
      }

      if (ch == '\\') {
        escaped = !escaped;
      } else {
        escaped = false;
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

  absl::StatusOr<std::unique_ptr<Token>> Lexer::ReadVariable() {
    int start = index_;
    Increment();
    while(CanContinue()) {
      char ch = CurrentChar();
      if (!isalnum(ch) && ch != '_') {
        break;
      }
      Increment();
    }
    return MakeToken(TokenType::kVariable, start);
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
    if (isdigit(ch) || ch == '.') return ReadNumber();
    if (isalpha(ch) || ch == '_') return ReadVariable();

    if (start >= index_) {
      return absl::FailedPreconditionError(absl::Substitute("start[$0] < index_[$1]", start, index_));
    }
    return nullptr;
  }

}
