#pragma once

#include "ast/term.h"

namespace ast {

class symbol_item : public term {
public:
  symbol_item(node *current, symbol_item *next)
      : _current(current), _next(next) {}

private:
  node *_current;
  symbol_item *_next;
};

class symbol : public node {
  enum class type { axiom, ignore };

public:
  symbol(symbol_item *first, type type) : _first(first), _type(type) {}

private:
  symbol_item *_first;
  type _type;
};
} // namespace ast
