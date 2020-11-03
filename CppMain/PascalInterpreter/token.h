#ifndef TOKEN_H
#define TOKEN_H

#include "absl/strings/string_view.h"

namespace xiangyuliu::parser {

enum class TokenType {
  kNumber,
  kOperator,
  kString,
};

std::string ToTokenTypeString(const TokenType& type);

class Token {
 public:
  Token(const TokenType type, absl::string_view content): type_(type), content_(content) {}

  const TokenType& type() const {return type_;}
  absl::string_view content() const { return content_; }

private:
  const TokenType type_;
  const absl::string_view content_;
};

}
#endif
