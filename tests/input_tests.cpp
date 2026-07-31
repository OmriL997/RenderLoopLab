#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "renderlooplab/input.hpp"

#include <cmath>

using namespace rll;
using Approx = Catch::Approx;

TEST_CASE("InputState: default state is all false", "[input]")
{
    InputState s;
    for (int i = 0; i < static_cast<int>(InputAction::Count); ++i) {
        const auto a = static_cast<InputAction>(i);
        REQUIRE_FALSE(s.isPressed(a));
        REQUIRE_FALSE(s.isHeld(a));
    }
    const glm::vec2 mv = s.getMovement();
    REQUIRE(mv.x == Approx(0.0f));
    REQUIRE(mv.y == Approx(0.0f));
}

TEST_CASE("InputState: setPressed / isPressed", "[input]")
{
    InputState s;
    s.setPressed(InputAction::MoveLeft, true);
    REQUIRE(s.isPressed(InputAction::MoveLeft));
    REQUIRE_FALSE(s.isPressed(InputAction::MoveRight));
}

TEST_CASE("InputState: setHeld / isHeld", "[input]")
{
    InputState s;
    s.setHeld(InputAction::MoveRight, true);
    REQUIRE(s.isHeld(InputAction::MoveRight));
    REQUIRE_FALSE(s.isHeld(InputAction::MoveLeft));
}

TEST_CASE("InputState: resetPerFrame clears pressed but not held", "[input]")
{
    InputState s;
    s.setPressed(InputAction::MoveUp, true);
    s.setHeld   (InputAction::MoveUp, true);

    s.resetPerFrame();

    REQUIRE_FALSE(s.isPressed(InputAction::MoveUp));  // pressed is cleared
    REQUIRE(s.isHeld(InputAction::MoveUp));           // held persists
}

TEST_CASE("InputState: held clears explicitly", "[input]")
{
    InputState s;
    s.setHeld(InputAction::MoveDown, true);
    REQUIRE(s.isHeld(InputAction::MoveDown));
    s.setHeld(InputAction::MoveDown, false);
    REQUIRE_FALSE(s.isHeld(InputAction::MoveDown));
}

TEST_CASE("InputState: diagonal movement is normalized", "[input]")
{
    InputState s;
    s.setHeld(InputAction::MoveRight, true);
    s.setHeld(InputAction::MoveDown,  true);

    const glm::vec2 mv = s.getMovement();
    const float len = std::sqrt(mv.x * mv.x + mv.y * mv.y);
    REQUIRE(len == Approx(1.0f).epsilon(1e-5f));
}

TEST_CASE("InputState: single-axis movement has unit length", "[input]")
{
    InputState s;
    s.setHeld(InputAction::MoveLeft, true);
    const glm::vec2 mv = s.getMovement();
    REQUIRE(mv.x == Approx(-1.0f));
    REQUIRE(mv.y == Approx(0.0f));
}

TEST_CASE("InputState: analog movement dead-zone passthrough", "[input]")
{
    InputState s;
    // Set analog movement (already dead-zoned by SdlInput before being stored)
    s.setAnalogMovement({0.5f, 0.0f});
    const glm::vec2 mv = s.getAnalogMovement();
    REQUIRE(mv.x == Approx(0.5f));
    REQUIRE(mv.y == Approx(0.0f));
}

TEST_CASE("InputState: combined keyboard + analog is clamped to unit", "[input]")
{
    InputState s;
    s.setHeld(InputAction::MoveRight, true);  // digital +1, 0
    s.setAnalogMovement({1.0f, 0.0f});        // analog +1, 0

    const glm::vec2 mv = s.getMovement();
    const float len = std::sqrt(mv.x * mv.x + mv.y * mv.y);
    REQUIRE(len <= 1.0f + 1e-5f);
}

TEST_CASE("InputState: no movement when no input", "[input]")
{
    InputState s;
    const glm::vec2 mv = s.getMovement();
    REQUIRE(mv.x == Approx(0.0f));
    REQUIRE(mv.y == Approx(0.0f));
}

TEST_CASE("InputState: Quit action pressed and held", "[input]")
{
    InputState s;
    s.setPressed(InputAction::Quit, true);
    s.setHeld   (InputAction::Quit, true);
    REQUIRE(s.isPressed(InputAction::Quit));
    REQUIRE(s.isHeld   (InputAction::Quit));
    s.resetPerFrame();
    REQUIRE_FALSE(s.isPressed(InputAction::Quit));
    REQUIRE      (s.isHeld   (InputAction::Quit));
}
