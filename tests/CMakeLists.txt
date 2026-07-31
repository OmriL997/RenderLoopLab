add_executable(renderlooplab_tests
    fixed_timestep_tests.cpp
    input_tests.cpp
    simulation_tests.cpp
    frame_profiler_tests.cpp
    cli_tests.cpp
    benchmark_report_tests.cpp
    resource_traits_tests.cpp
)

target_link_libraries(renderlooplab_tests
    PRIVATE renderlooplab_core
    PRIVATE glad           # provides glad/glad.h for resource_traits_tests (no GL calls made)
    PRIVATE Catch2::Catch2WithMain
    PRIVATE glm::glm
)

rll_set_warnings(renderlooplab_tests)
rll_enable_sanitizers(renderlooplab_tests)

# Register with CTest — use add_test directly to avoid catch_discover_tests
# grouping all tests into one semicolon-joined CTest entry on Windows.
add_test(NAME renderlooplab_tests COMMAND renderlooplab_tests)
