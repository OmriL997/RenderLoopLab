#pragma once

#include <string_view>

namespace rll::log {

enum class Level { Info, Warn, Error };

void log(Level level, std::string_view category, std::string_view message);

inline void info (std::string_view cat, std::string_view msg) { log(Level::Info,  cat, msg); }
inline void warn (std::string_view cat, std::string_view msg) { log(Level::Warn,  cat, msg); }
inline void error(std::string_view cat, std::string_view msg) { log(Level::Error, cat, msg); }

} // namespace rll::log
