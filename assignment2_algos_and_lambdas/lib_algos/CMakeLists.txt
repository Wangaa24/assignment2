cmake_minimum_required(VERSION 3.8)

set(lib_name lib_algos)

project(${lib_name} LANGUAGES CXX)

set(HEADERS_FILES_LIB
        ${CMAKE_CURRENT_SOURCE_DIR}/include/algos.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/task.h
        )

add_library(${lib_name} INTERFACE)

target_sources(${lib_name}
        INTERFACE
            ${HEADERS_FILES_LIB}
        )

target_include_directories(${lib_name} 
        INTERFACE 
            ${CMAKE_CURRENT_SOURCE_DIR}/include/
        )

target_compile_features(${lib_name} INTERFACE cxx_std_17)
set_target_properties(${lib_name} PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(${lib_name} INTERFACE
        # set warnings for all build types
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic -Werror -fno-omit-frame-pointer>
        # debug optimisation in Debug
        $<$<AND:$<CXX_COMPILER_ID:GNU,Clang,AppleClang>,$<CONFIG:Release>>:-O3>
        # O3 optimisation Release
        $<$<AND:$<CXX_COMPILER_ID:GNU,Clang,AppleClang>,$<CONFIG:Debug>>:-Og -g>
        )

# Set options for MSVC
target_compile_options(${lib_name} INTERFACE
        # set warnings for all build types
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        # debug optimisation in Debug
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/O2>
        # O3 optimisation in Release
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/RTC1 /Od /Zi>
        )