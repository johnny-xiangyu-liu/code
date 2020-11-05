// expression.h
//
// last-edit-by: <>
//
// Description:
//
//////////////////////////////////////////////////////////////////////

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <memory>
#include <string>
#include <vector>

#include "absl/strings/string_view.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"


namespace xiangyuliu::parser{

class Expression {
 public:
  Expression(Token token) : token_(token) {}

  void Add(std::unique_ptr<Expression> child) {
    children_.push_back(std::move(child));
  }

 private:
  Token token_;
  std::vector<std::unique_ptr<Expression>> children_;
};

}

#endif // EXPRESSION_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
