#include <Utils/CSSUtil.hpp>

#include <fstream>
#include <sstream>

#include <fmt/format.h>
#include <Utils/ProcessUtil.hpp>

#include <Application.hpp>

using namespace Util::CSS;

std::optional<std::string> Util::CSS::SCSStoCSS(std::string scss, std::string includePath) {
    return Util::Process::runCommand(fmt::format("echo '{}' | sass --stdin --load-path='{}'", scss, includePath));
}

std::optional<std::string> Util::CSS::loadCSSFromFile(std::string path, std::string includePath) {
    std::ifstream file(path);
    if(!file.is_open()) {
        return std::nullopt;
    }

    std::stringstream stream;
    stream << file.rdbuf();

    if(path.substr(std::max(0L, ((int64_t)path.size()) - 5)) == ".scss") {
        return SCSStoCSS(stream.str(), includePath);
    }

    return stream.str();
}