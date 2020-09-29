# Mbed CMake Template
# Copyright 2020 Ladislas de Toldi (ladislas [at] detoldi.me)
# SPDX-License-Identifier: Apache-2.0

# mbed-cmake requires at least CMake 3.18
cmake_minimum_required(VERSION 3.18)

# ⚠️ Before all, include mbed-cmake.cmake
include(./mbed-cmake.cmake)

# Then configure name of the project
project(LekaOS LANGUAGES C CXX ASM)

# For convenience you can define useful variables
set(LIB_DIR ${ROOT_DIR}/lib)
set(TARGETS_DIR ${ROOT_DIR}/targets)

# Add custom targets
if (${TARGET_BOARD} MATCHES "\-x")
	string(REPLACE "\-x " "" CUSTOM_BOARD ${TARGET_BOARD})
	add_subdirectory(${TARGETS_DIR}/TARGET_${CUSTOM_BOARD})
endif()

# Add libraries
add_subdirectory(${LIB_DIR}/HelloWorld)

# Add the main source files of the project from ./src
add_subdirectory(${ROOT_DIR}/src)

# If you use spikes to test features, you can add them here
add_subdirectory(${ROOT_DIR}/spike/blinky)

# Finally print the mbed-cmake build report
mbed_cmake_print_build_report()
