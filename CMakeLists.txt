# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

# mbed-cmake requires at least CMake 3.18
cmake_minimum_required(VERSION 3.18)

# Activate ccache if available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

# Before all, set ROOT_DIR, MBED_OS_DIR
set(ROOT_DIR    ${CMAKE_CURRENT_LIST_DIR})
set(MBED_OS_DIR ${ROOT_DIR}/extern/mbed-os)

# And include mbed-cmake.cmake
include(./mbed-cmake.cmake)

# Then configure name of the project
project(LekaOS LANGUAGES C CXX ASM)

# Generate compile commands database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# For convenience you can define useful variables
set(OS_DIR      ${ROOT_DIR}/src)
set(LIBS_DIR    ${ROOT_DIR}/libs)
set(TESTS_DIR   ${ROOT_DIR}/tests)
set(SPIKES_DIR  ${ROOT_DIR}/spikes)
set(DRIVERS_DIR ${ROOT_DIR}/drivers)
set(TARGETS_DIR ${ROOT_DIR}/targets)

# Add custom target subdirectory
add_target_board_subdirectory("${TARGET_BOARD}")

# Add drivers & libraries
add_subdirectory(${DRIVERS_DIR})
add_subdirectory(${LIBS_DIR})

# Add spikes, functional tests
add_subdirectory(${SPIKES_DIR})
add_subdirectory(${TESTS_DIR})

# Add LekaOS
add_subdirectory(${OS_DIR})

# Finally print the mbed-cmake build report
mbed_cmake_print_build_report()
