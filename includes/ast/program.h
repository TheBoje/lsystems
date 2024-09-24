#pragma once

#include "ast/term.h"

class visitor;

namespace ast {

class program : node {
public:
  program() {}
  program(program *program, term *item) : _item(item) { (void)program;}
  ~program() { delete _item; }

  virtual void accept(visitor &) { (void)_item; }

private:
  term *_item = nullptr;
};

} // namespace ast
