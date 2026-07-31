#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "renderlooplab/simulation.hpp"
#include "renderlooplab/input.hpp"

#include <cmath>

using namespace rll;
using Approx = Catch::Approx;

static SimulationConfig makeConfig(uint64_t seed = 12345, int n = 10)
{
    SimulationConfig c;
    c.seed        = seed;
    c.numSprites  = n;
    c.worldBounds = {800.0f, 600.0f};
    return c;
}

TEST_CASE("Simulation: same seed produces same initial state", "[simulation]")
{
    Simulation a(makeConfig(42));
    Simulation b(makeConfig(42));

    REQUIRE(a.sprites().size() == b.sprites().size());
    for (std::size_t i = 0; i < a.sprites().size(); ++i) {
        REQUIRE(a.sprites()[i].currentPosition.x ==
                Approx(b.sprites()[i].currentPosition.x));
        REQUIRE(a.sprites()[i].currentPosition.y ==
                Approx(b.sprites()[i].currentPosition.y));
    }
}

TEST_CASE("Simulation: different seeds produce different initial states", "[simulation]")
{
    Simulation a(makeConfig(1));
    Simulation b(makeConfig(2));

    // At least one sprite position should differ
    bool differs = false;
    for (std::size_t i = 1; i < a.sprites().size(); ++i) {
        if (a.sprites()[i].currentPosition.x !=
            Approx(b.sprites()[i].currentPosition.x)) {
            differs = true;
            break;
        }
    }
    REQUIRE(differs);
}

TEST_CASE("Simulation: same seed + same updates = same final state", "[simulation]")
{
    auto cfg = makeConfig(99, 20);
    Simulation a(cfg), b(cfg);
    InputState input;

    for (int i = 0; i < 100; ++i) {
        a.update(1.0 / 60.0, input);
        b.update(1.0 / 60.0, input);
    }

    for (std::size_t i = 0; i < a.sprites().size(); ++i) {
        REQUIRE(a.sprites()[i].currentPosition.x ==
                Approx(b.sprites()[i].currentPosition.x));
        REQUIRE(a.sprites()[i].currentPosition.y ==
                Approx(b.sprites()[i].currentPosition.y));
    }
}

TEST_CASE("Simulation: sprites[0] is the player", "[simulation]")
{
    Simulation sim(makeConfig());
    REQUIRE_FALSE(sim.sprites().empty());
    REQUIRE(sim.sprites()[0].isPlayer);
}

TEST_CASE("Simulation: background sprites are not the player", "[simulation]")
{
    Simulation sim(makeConfig(1, 5));
    for (std::size_t i = 1; i < sim.sprites().size(); ++i)
        REQUIRE_FALSE(sim.sprites()[i].isPlayer);
}

TEST_CASE("Simulation: sprites stay within world bounds", "[simulation]")
{
    auto cfg = makeConfig(7, 30);
    Simulation sim(cfg);
    InputState input;
    const float W = cfg.worldBounds.x;
    const float H = cfg.worldBounds.y;

    for (int step = 0; step < 300; ++step)
        sim.update(1.0 / 60.0, input);

    for (const auto& s : sim.sprites()) {
        REQUIRE(s.currentPosition.x >= 0.0f);
        REQUIRE(s.currentPosition.y >= 0.0f);
        REQUIRE(s.currentPosition.x <= W);
        REQUIRE(s.currentPosition.y <= H);
    }
}

TEST_CASE("Simulation: previousPosition updated each step", "[simulation]")
{
    Simulation sim(makeConfig(5, 5));
    InputState input;
    const auto prevBefore = sim.sprites()[1].currentPosition;

    sim.update(1.0 / 60.0, input);

    // After update, previousPosition should equal what currentPosition was before
    REQUIRE(sim.sprites()[1].previousPosition.x == Approx(prevBefore.x));
    REQUIRE(sim.sprites()[1].previousPosition.y == Approx(prevBefore.y));
}

TEST_CASE("Simulation: player responds to input", "[simulation]")
{
    auto cfg = makeConfig();
    Simulation sim(cfg);

    InputState leftInput;
    leftInput.setHeld(InputAction::MoveLeft, true);

    const float startX = sim.sprites()[0].currentPosition.x;
    for (int i = 0; i < 10; ++i)
        sim.update(1.0 / 60.0, leftInput);

    REQUIRE(sim.sprites()[0].currentPosition.x < startX);
}

TEST_CASE("Simulation: sprite count matches config", "[simulation]")
{
    constexpr int N = 42;
    Simulation sim(makeConfig(0, N));
    REQUIRE(static_cast<int>(sim.sprites().size()) == N);
}

TEST_CASE("Simulation: interpolation alpha=0 gives previous, alpha=1 gives current",
          "[simulation]")
{
    Simulation sim(makeConfig(3, 5));
    InputState input;
    sim.update(1.0 / 60.0, input);

    const auto& s = sim.sprites()[1];
    // alpha=0: interpolated position == previousPosition
    const glm::vec2 atZero  = s.previousPosition * 1.0f + s.currentPosition * 0.0f;
    const glm::vec2 atOne   = s.previousPosition * 0.0f + s.currentPosition * 1.0f;

    REQUIRE(atZero.x == Approx(s.previousPosition.x));
    REQUIRE(atOne.x  == Approx(s.currentPosition.x));
}
