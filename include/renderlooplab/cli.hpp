#pragma once

#include "renderlooplab/application_config.hpp"

#include <optional>
#include <string>

namespace rll {

struct CliResult {
    bool              showHelp = false;
    ApplicationConfig config;
};

/**
 * Parse argc/argv into an ApplicationConfig.
 *
 * Returns std::nullopt and fills errorOut on any parse failure.
 * Returns CliResult{showHelp=true} when --help/-h is requested.
 *
 * Exit-code contract (caller's responsibility):
 *   showHelp=true  → print help, exit 0
 *   nullopt        → print errorOut, exit 1
 *   otherwise      → proceed with config
 */
std::optional<CliResult> parseCli(int argc, char* argv[], std::string& errorOut);

void printHelp(const char* programName);

} // namespace rll
