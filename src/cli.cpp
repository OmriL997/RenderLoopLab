#include "renderlooplab/cli.hpp"
#include "renderlooplab/version.hpp"

#include <cstring>
#include <stdexcept>
#include <string>

namespace rll {

namespace {

bool nextArg(int argc, char* argv[], int& i, std::string_view flag,
             std::string& valueOut, std::string& errorOut)
{
    if (i + 1 >= argc) {
        errorOut = std::string("Option '") + std::string(flag) + "' requires a value.";
        return false;
    }
    valueOut = argv[++i];
    return true;
}

bool parseInt(const std::string& s, int& out, std::string_view flag, std::string& err)
{
    try {
        std::size_t pos = 0;
        long v = std::stol(s, &pos);
        if (pos != s.size()) throw std::invalid_argument("trailing chars");
        out = static_cast<int>(v);
        return true;
    } catch (...) {
        err = std::string("Invalid integer for '") + std::string(flag) + "': " + s;
        return false;
    }
}

bool parseUInt64(const std::string& s, uint64_t& out, std::string_view flag, std::string& err)
{
    try {
        std::size_t pos = 0;
        unsigned long long v = std::stoull(s, &pos);
        if (pos != s.size()) throw std::invalid_argument("trailing chars");
        out = static_cast<uint64_t>(v);
        return true;
    } catch (...) {
        err = std::string("Invalid unsigned integer for '") + std::string(flag) + "': " + s;
        return false;
    }
}

bool parseDouble(const std::string& s, double& out, std::string_view flag, std::string& err)
{
    try {
        std::size_t pos = 0;
        out = std::stod(s, &pos);
        if (pos != s.size()) throw std::invalid_argument("trailing chars");
        return true;
    } catch (...) {
        err = std::string("Invalid number for '") + std::string(flag) + "': " + s;
        return false;
    }
}

} // anonymous namespace

// ---------------------------------------------------------------------------

std::optional<CliResult> parseCli(int argc, char* argv[], std::string& errorOut)
{
    CliResult result;
    ApplicationConfig& cfg = result.config;

    for (int i = 1; i < argc; ++i) {
        std::string_view arg = argv[i];
        std::string val;

        // ── Help ────────────────────────────────────────────────────────
        if (arg == "--help" || arg == "-h") {
            result.showHelp = true;
            return result;
        }

        // ── Mode flags ───────────────────────────────────────────────────
        else if (arg == "--benchmark") {
            cfg.benchmark = true;
            // Apply benchmark defaults when not already overridden later
            if (cfg.sprites == ApplicationConfig{}.sprites)
                cfg.sprites = 1000;
        }
        else if (arg == "--smoke-test") {
            cfg.smokeTest = true;
        }
        else if (arg == "--no-vsync") {
            cfg.vsync = false;
        }
        else if (arg == "--hidden") {
            cfg.hidden = true;
        }

        // ── Numeric options ──────────────────────────────────────────────
        else if (arg == "--sprites") {
            if (!nextArg(argc, argv, i, arg, val, errorOut)) return std::nullopt;
            int n = 0;
            if (!parseInt(val, n, arg, errorOut)) return std::nullopt;
            if (n < ApplicationConfig::MIN_SPRITES) {
                errorOut = "--sprites must be >= " + std::to_string(ApplicationConfig::MIN_SPRITES);
                return std::nullopt;
            }
            if (n > ApplicationConfig::MAX_SPRITES) {
                errorOut = "--sprites must be <= " + std::to_string(ApplicationConfig::MAX_SPRITES);
                return std::nullopt;
            }
            cfg.sprites = n;
        }
        else if (arg == "--frames") {
            if (!nextArg(argc, argv, i, arg, val, errorOut)) return std::nullopt;
            int n = 0;
            if (!parseInt(val, n, arg, errorOut)) return std::nullopt;
            if (n < 1) { errorOut = "--frames must be >= 1"; return std::nullopt; }
            cfg.measuredFrames = n;
        }
        else if (arg == "--warmup") {
            if (!nextArg(argc, argv, i, arg, val, errorOut)) return std::nullopt;
            int n = 0;
            if (!parseInt(val, n, arg, errorOut)) return std::nullopt;
            if (n < 0) { errorOut = "--warmup must be >= 0"; return std::nullopt; }
            cfg.warmupFrames = n;
        }
        else if (arg == "--seed") {
            if (!nextArg(argc, argv, i, arg, val, errorOut)) return std::nullopt;
            uint64_t s = 0;
            if (!parseUInt64(val, s, arg, errorOut)) return std::nullopt;
            cfg.seed = s;
        }
        else if (arg == "--fixed-hz") {
            if (!nextArg(argc, argv, i, arg, val, errorOut)) return std::nullopt;
            double hz = 0.0;
            if (!parseDouble(val, hz, arg, errorOut)) return std::nullopt;
            if (hz < ApplicationConfig::MIN_FIXED_HZ || hz > ApplicationConfig::MAX_FIXED_HZ) {
                errorOut = "--fixed-hz must be in [" +
                           std::to_string(ApplicationConfig::MIN_FIXED_HZ) + ", " +
                           std::to_string(ApplicationConfig::MAX_FIXED_HZ) + "]";
                return std::nullopt;
            }
            cfg.fixedHz = hz;
        }
        else if (arg == "--width") {
            if (!nextArg(argc, argv, i, arg, val, errorOut)) return std::nullopt;
            int n = 0;
            if (!parseInt(val, n, arg, errorOut)) return std::nullopt;
            if (n < 1) { errorOut = "--width must be >= 1"; return std::nullopt; }
            cfg.width = n;
        }
        else if (arg == "--height") {
            if (!nextArg(argc, argv, i, arg, val, errorOut)) return std::nullopt;
            int n = 0;
            if (!parseInt(val, n, arg, errorOut)) return std::nullopt;
            if (n < 1) { errorOut = "--height must be >= 1"; return std::nullopt; }
            cfg.height = n;
        }

        // ── String / path options ────────────────────────────────────────
        else if (arg == "--csv") {
            if (!nextArg(argc, argv, i, arg, val, errorOut)) return std::nullopt;
            cfg.csvPath = val;
        }
        else if (arg == "--report") {
            if (!nextArg(argc, argv, i, arg, val, errorOut)) return std::nullopt;
            cfg.reportPath = val;
        }
        else if (arg == "--asset-root") {
            if (!nextArg(argc, argv, i, arg, val, errorOut)) return std::nullopt;
            cfg.assetRoot = val;
        }

        // ── Unknown ──────────────────────────────────────────────────────
        else {
            errorOut = "Unknown argument: ";
            errorOut += std::string(arg);
            return std::nullopt;
        }
    }

    // ── Post-parse validation ────────────────────────────────────────────
    if ((cfg.benchmark || cfg.smokeTest) && cfg.measuredFrames == 0) {
        // Apply sensible defaults
        if (cfg.benchmark && cfg.measuredFrames == 0)
            cfg.measuredFrames = 3000;
        if (cfg.smokeTest  && cfg.measuredFrames == 0)
            cfg.measuredFrames = 120;
    }

    return result;
}

// ---------------------------------------------------------------------------

void printHelp(const char* programName)
{
    std::printf(
        "RenderLoopLab v%s\n"
        "Real-time 2D rendering and game-loop laboratory\n\n"
        "Usage:\n"
        "  %s                       Interactive mode\n"
        "  %s --benchmark [OPTIONS]  Benchmark mode (exits automatically)\n"
        "  %s --smoke-test [OPTIONS] CI smoke test (exits automatically)\n\n"
        "Options:\n"
        "  --sprites <n>       Number of sprites [1, 100000]. Default: 500 (interactive), 1000 (benchmark)\n"
        "  --frames <n>        Measured frames before exit. Default: 3000 (benchmark), 120 (smoke-test)\n"
        "  --warmup <n>        Unmeasured warm-up frames. Default: 120\n"
        "  --seed <n>          Deterministic RNG seed. Default: 12345\n"
        "  --fixed-hz <hz>     Fixed simulation frequency. Default: 60\n"
        "  --width <px>        Window width. Default: 1280\n"
        "  --height <px>       Window height. Default: 720\n"
        "  --no-vsync          Disable VSync\n"
        "  --hidden            Create a hidden window (CI / benchmark)\n"
        "  --csv <path>        Export per-frame CSV\n"
        "  --report <path>     Export benchmark JSON report\n"
        "  --asset-root <dir>  Override shader asset directory\n"
        "  -h, --help          Show this help\n\n"
        "Exit codes:\n"
        "  0  Success\n"
        "  1  Command-line / configuration error\n"
        "  2  SDL / OpenGL / shader initialisation failure\n"
        "  3  Benchmark report write failure\n"
        "  4  Smoke-test runtime failure\n",
        VERSION_STRING,
        programName, programName, programName);
}

} // namespace rll
