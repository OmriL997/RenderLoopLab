# cmake/Sanitizers.cmake
# Enables AddressSanitizer + UndefinedBehaviorSanitizer on GCC/Clang.
# MSVC sanitizer support is partial; skip it to avoid link issues in CI.

function(rll_enable_sanitizers target)
    if(NOT RLL_ENABLE_SANITIZERS)
        return()
    endif()

    if(MSVC)
        message(WARNING "Sanitizers requested but MSVC does not fully support ASan+UBSan via this path; skipping.")
        return()
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
        )
        target_link_options(${target} PRIVATE
            -fsanitize=address,undefined
        )
        message(STATUS "Sanitizers (ASan + UBSan) enabled on target: ${target}")
    else()
        message(WARNING "Sanitizers requested but compiler '${CMAKE_CXX_COMPILER_ID}' is not supported; skipping.")
    endif()
endfunction()
