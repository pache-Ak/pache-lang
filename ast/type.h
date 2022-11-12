#ifndef TYPE_H
#define TYPE_H

#include "ast.h"

namespace pache {
  enum class value_category {
    prvalue,
    xvalue,
    lvalue,
  };




  class primary_type : public type_ast {
  public:

  };

  class I32_type : public primary_type {

  };


}