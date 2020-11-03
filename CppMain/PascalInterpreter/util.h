// util.h
//
// last-edit-by: <>
//
// Description:
//
//////////////////////////////////////////////////////////////////////

#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>

namespace xiangyuliu {
  inline void CHECK(bool condition, std::string msg = "") {
    if (!condition) {
      std::cerr << msg << std::endl;
      std::exit(0);
    }
  }
}

#endif // UTIL_H
