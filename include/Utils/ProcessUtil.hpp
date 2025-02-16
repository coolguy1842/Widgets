#ifndef __PROCESS_UTIL_HPP__
#define __PROCESS_UTIL_HPP__


#include <cstdint>
#include <string>
#include <optional>

namespace Util::Process {

std::optional<std::string> runCommand(const std::string command, const uint64_t bufSize = 256);

};

#endif