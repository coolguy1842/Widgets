#ifndef __CSS_UTIL_HPP__
#define __CSS_UTIL_HPP__

#include <string>
#include <optional>

namespace Util::CSS {

std::optional<std::string> SCSStoCSS(std::string scss, std::string includePath = "./");
std::optional<std::string> loadCSSFromFile(std::string path, std::string includePath = "./");

};

#endif