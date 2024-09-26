#pragma once

#include <iostream>
#include <string>

namespace ast {

class configuration {
public:
  configuration() {}

  friend std::ostream &operator<<(std::ostream &os, const configuration &conf) {
    os << "\tderivation: " << conf.derivation << std::endl
       << "\tangle factor: " << conf.angle_factor << std::endl
       << "\tscale factor: " << conf.scale_factor << std::endl
       << "\taxiom: " << conf.axiom << std::endl
       << "\tignore: " << conf.ignore << std::endl;
    return os;
  }

public:
  int derivation;
  float angle_factor;
  float scale_factor;
  std::string axiom;
  std::string ignore;
};

} // namespace ast
