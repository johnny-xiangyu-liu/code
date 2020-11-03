#include "token.h"

namespace xiangyuliu::parser {

  std::string ToTokenTypeString(const TokenType& type) {
    switch (type) {
    case TokenType::kNumber:
      return "Number";
    case TokenType::kString:
      return "String";
    case TokenType::kOperator:
      return "Operator";
    }
    return "failed";
  }

}
