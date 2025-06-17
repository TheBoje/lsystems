#pragma once

#include <iostream>
#include <string>

namespace ast {

class configuration {
public:
	configuration() { }

	friend std::ostream& operator<<(std::ostream& os, const configuration& conf);

public:
	int derivation = 1;
	float angle_factor = 1.;
	float scale_factor = 1.;
	std::string axiom;
	std::string ignore; // optionnal
	std::string result;
};

} // namespace ast
