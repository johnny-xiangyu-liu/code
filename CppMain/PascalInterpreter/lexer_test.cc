#include "lexer.h"

#include <string>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "token.h"

namespace xiangyuliu::parser {
namespace {

  using ::testing::Pointee;
  using ::testing::Eq;
  using ::testing::StrEq;

  MATCHER_P2(EqToken, type, content, "Match Token with [" + ToTokenTypeString(type) + "] and content[" + content + "]") {
   if (ExplainMatchResult(Eq(type), arg.type(), result_listener) &&
       ExplainMatchResult(StrEq(content), arg.content(), result_listener)) {
     return true;
   }

   *result_listener << "the difference: [" << ToTokenTypeString(arg.type()) << "]:[" << arg.content() << "]";
   return false;
  }

  MATCHER_P(EqToken, expected, "Match Token") {
   return
     EqToken(expected.type(), expected.content());
  }

  TEST(Lexer, SimpleMath) {
    Lexer target(" 1 + 2 - 3 / 4 * 5 % 6 ");
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kNumber, "1")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kOperator, "+")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kNumber, "2")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kOperator, "-")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kNumber, "3")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kOperator, "/")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kNumber, "4")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kOperator, "*")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kNumber, "5")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kOperator, "%")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kNumber, "6")));
  }

  TEST(Lexer, StringToken) {
    Lexer target(" \" foo bar \"");
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kString, " foo bar ")));
  }

  TEST(Lexer, HasMoreToken) {
    std::string simple_string[] =
      {" 1 + 2", "1+2", " 1 + 2 "};
    for (const std::string& str : simple_string) {
    Lexer target(str);
    EXPECT_TRUE(target.HasMoreToken());
    target.ReadToken();
    EXPECT_TRUE(target.HasMoreToken());
    target.ReadToken();
    EXPECT_TRUE(target.HasMoreToken());
    target.ReadToken();
    EXPECT_FALSE(target.HasMoreToken());
    }
  }

}  // namespace
}  // namespace xiangyuliu::parser
