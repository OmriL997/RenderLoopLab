#include "renderlooplab/logging.hpp"

#include <cstdio>

namespace rll::log {

void log(Level level, std::string_view category, std::string_view message)
{
    const char* prefix = nullptr;
    switch (level) {
        case Level::Info:  prefix = "INFO";    break;
        case Level::Warn:  prefix = "WARNING"; break;
        case Level::Error: prefix = "ERROR";   break;
    }
    FILE* out = (level == Level::Error) ? stderr : stdout;
    std::fprintf(out, "%s [%.*s] %.*s\n",
        prefix,
        static_cast<int>(category.size()), category.data(),
        static_cast<int>(message.size()),  message.data());
    std::fflush(out);
}

} // namespace rll::log
