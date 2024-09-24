#pragma once

#include "ast/term.h"

class visitor;

namespace ast {

class definition : public term {
public:
  enum class type { derivation, angle_factor, scale };

  union definition_value {
    int i;
    float f;
  };

  definition(int value, type type) : _type(type) { _value.i = value; };
  definition(float value, type type) : _type(type) { _value.f = value; };
  ~definition() {};

  inline int get_int() const { return _value.i; }
  inline float get_float() const { return _value.f; }
  inline type get_type() const { return _type; }

  virtual void accept(visitor &) {};

private:
  definition_value _value;
  type _type;
};

} // namespace ast
