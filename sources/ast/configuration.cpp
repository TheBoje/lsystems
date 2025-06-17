#include "ast/configuration.h"

namespace ast {

std::ostream& operator<<(std::ostream& os, const configuration& conf) {
	os << "\tderivation: " << conf.derivation << std::endl
	   << "\tangle factor: " << conf.angle_factor << std::endl
	   << "\tscale factor: " << conf.scale_factor << std::endl
	   << "\taxiom: " << conf.axiom << std::endl
	   << "\tignore: " << (!conf.ignore.empty() ? conf.ignore : "none") << std::endl;
	return os;
}

} // namespace ast
