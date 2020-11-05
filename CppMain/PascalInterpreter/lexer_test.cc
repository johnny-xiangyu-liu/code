#include "lexer.h"

#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "absl/status/status.h"
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

  MATCHER_P(IsOkAndHolds, a_matcher, "IsOkAndHolds") {
    if (!arg.ok()) {
      *result_listener << "StatusOr is not ok:" << arg.status().ToString();
      return false;
    }
    return ExplainMatchResult(a_matcher, arg.value(), result_listener);
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

  TEST(Lexer, NumberToken) {
    Lexer target(R"raw(1 1. .1 1.0 0.1 0.001)raw");
    EXPECT_THAT(target.ReadToken(),
                IsOkAndHolds(Pointee(EqToken(TokenType::kNumber, "1"))));
    EXPECT_THAT(target.ReadToken(),
                IsOkAndHolds(Pointee(EqToken(TokenType::kNumber, "1."))));
    EXPECT_THAT(target.ReadToken(),
                IsOkAndHolds(Pointee(EqToken(TokenType::kNumber, ".1"))));
    EXPECT_THAT(target.ReadToken(),
                IsOkAndHolds(Pointee(EqToken(TokenType::kNumber, "1.0"))));
    EXPECT_THAT(target.ReadToken(),
                IsOkAndHolds(Pointee(EqToken(TokenType::kNumber, "0.1"))));
    EXPECT_THAT(target.ReadToken(),
                IsOkAndHolds(Pointee(EqToken(TokenType::kNumber, "0.001"))));
  }

  TEST(Lexer, NumberTokenFailed) {
    std::string failed_numbers[] = {
      "1..", "..1", "0.0001.", "0.0.1"
    };
    for (const auto& str : failed_numbers) {
      Lexer target(str);
      auto status_or = target.ReadToken();
      EXPECT_FALSE(status_or.ok());
      EXPECT_THAT(status_or.status().code(), Eq(absl::StatusCode::kFailedPrecondition));
      EXPECT_THAT(status_or.status().message(), StrEq("Malformed number. a '.' has occurred already."));
      }
  }
  TEST(Lexer, StringToken) {
    Lexer target(R"raw(" foo bar " "baz\" """)raw");
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kString, " foo bar ")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kString, "baz\\\" ")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kString, "")));
  }

  TEST(Lexxer, StringTokenFailed) {
    std::string failed[] = {
      "\"", "\"   ", "    \" abc "
    };
    for (const auto& str : failed) {
      Lexer target(str);
      EXPECT_FALSE(target.ReadToken().ok());
    }
  }

  TEST(Lexer, VariableToken) {
    Lexer target(R"raw( foo bar_123    bAz  __  _0_)raw");
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kVariable, "foo")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kVariable, "bar_123")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kVariable, "bAz")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kVariable, "__")));
    EXPECT_THAT(target.ReadToken().value(),
                Pointee(EqToken(TokenType::kVariable, "_0_")));
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
