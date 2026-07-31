# cmake/CompilerWarnings.cmake
# Sets project-level warnings on a target. Does NOT apply to third-party deps.

function(rll_set_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4
            /permissive-
            /utf-8        # treat source and execution charset as UTF-8
            /wd4251       # DLL-interface warning (not relevant here)
            /wd4996       # 'deprecated' functions
        )
        if(RLL_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wshadow
            -Wno-unused-parameter   # common in interface callbacks
        )
        if(RLL_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()
