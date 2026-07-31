#include <catch2/catch_test_macros.hpp>

#include "renderlooplab/cli.hpp"
#include "renderlooplab/application_config.hpp"

#include <vector>
#include <string>

using namespace rll;

// Build a char* argv[] from a list of string literals
struct FakeArgv {
    std::vector<std::string>  strs;
    std::vector<char*>        ptrs;

    explicit FakeArgv(std::initializer_list<const char*> args)
    {
        strs.reserve(args.size());
        ptrs.reserve(args.size());
        for (const char* s : args) {
            strs.emplace_back(s);
        }
        for (auto& s : strs) ptrs.push_back(s.data());
    }

    int   argc() const { return static_cast<int>(ptrs.size()); }
    char** argv()      { return ptrs.data(); }
};

TEST_CASE("CLI: default interactive config", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE(result.has_value());
    REQUIRE_FALSE(result->showHelp);
    const auto& cfg = result->config;
    REQUIRE(cfg.sprites   == 500);
    REQUIRE(cfg.seed      == 12345ULL);
    REQUIRE(cfg.fixedHz   == 60.0);
    REQUIRE(cfg.vsync     == true);
    REQUIRE(cfg.benchmark == false);
    REQUIRE(cfg.smokeTest == false);
}

TEST_CASE("CLI: --help returns showHelp=true", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--help"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE(result.has_value());
    REQUIRE(result->showHelp);
}

TEST_CASE("CLI: -h returns showHelp=true", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "-h"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE(result.has_value());
    REQUIRE(result->showHelp);
}

TEST_CASE("CLI: --benchmark sets benchmark mode and default sprites", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--benchmark"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE(result.has_value());
    REQUIRE(result->config.benchmark);
    REQUIRE(result->config.sprites == 1000);
}

TEST_CASE("CLI: --sprites override respected in benchmark mode", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--benchmark", "--sprites", "250"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE(result.has_value());
    REQUIRE(result->config.sprites == 250);
}

TEST_CASE("CLI: --smoke-test sets smoke test mode", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--smoke-test"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE(result.has_value());
    REQUIRE(result->config.smokeTest);
}

TEST_CASE("CLI: --no-vsync disables vsync", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--no-vsync"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE(result.has_value());
    REQUIRE_FALSE(result->config.vsync);
}

TEST_CASE("CLI: --hidden enables hidden mode", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--hidden"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE(result.has_value());
    REQUIRE(result->config.hidden);
}

TEST_CASE("CLI: --seed parses correctly", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--seed", "99999"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE(result.has_value());
    REQUIRE(result->config.seed == 99999ULL);
}

TEST_CASE("CLI: --frames parses correctly", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--frames", "500"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE(result.has_value());
    REQUIRE(result->config.measuredFrames == 500);
}

TEST_CASE("CLI: --warmup parses correctly", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--warmup", "60"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE(result.has_value());
    REQUIRE(result->config.warmupFrames == 60);
}

TEST_CASE("CLI: --fixed-hz parses correctly", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--fixed-hz", "120"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE(result.has_value());
    REQUIRE(result->config.fixedHz == 120.0);
}

TEST_CASE("CLI: --sprites 0 is rejected", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--sprites", "0"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE_FALSE(result.has_value());
    REQUIRE_FALSE(err.empty());
}

TEST_CASE("CLI: --sprites over MAX_SPRITES is rejected", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--sprites", "999999999"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("CLI: --fixed-hz 0 is rejected", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--fixed-hz", "0"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("CLI: --fixed-hz too large is rejected", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--fixed-hz", "10000"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("CLI: unknown argument is rejected", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--unknown-flag"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE_FALSE(result.has_value());
    REQUIRE_FALSE(err.empty());
}

TEST_CASE("CLI: option missing value is rejected", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--sprites"};  // no value
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("CLI: non-numeric sprites is rejected", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--sprites", "abc"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("CLI: --width and --height are parsed", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--width", "1920", "--height", "1080"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE(result.has_value());
    REQUIRE(result->config.width  == 1920);
    REQUIRE(result->config.height == 1080);
}

TEST_CASE("CLI: --csv and --report are parsed", "[cli]")
{
    FakeArgv fakeArgv{"renderlooplab", "--csv", "out.csv", "--report", "out.json"};
    std::string err;
    auto result = parseCli(fakeArgv.argc(), fakeArgv.argv(), err);
    REQUIRE(result.has_value());
    REQUIRE(result->config.csvPath    == "out.csv");
    REQUIRE(result->config.reportPath == "out.json");
}
