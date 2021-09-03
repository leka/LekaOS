# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

# mbed-cmake requires at least CMake 3.18
cmake_minimum_required(VERSION 3.18)

# Activate ccache if available
find_program(CCACHE "ccache")
if(CCACHE)
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE}")
endif(CCACHE)

option(ENABLE_CODE_ANALYSIS "Run code analysis" OFF)
message(STATUS "ENABLE_CODE_ANALYSIS --> ${ENABLE_CODE_ANALYSIS}")

if(ENABLE_CODE_ANALYSIS)
	find_program(CPPCHECK "cppcheck")
	if(CPPCHECK)
		set(CMAKE_CXX_CPPCHECK "${CPPCHECK}"
			"--platform=${CMAKE_CURRENT_SOURCE_DIR}/tools/config/cppcheck_leka_platform.xml"
			"--std=c++17"
			"--cppcheck-build-dir=build/cppcheck"
			"--enable=all"
			"--inconclusive"
			"--inline-suppr"
			"--quiet"
			"--suppress=unmatchedSuppression"
			"--suppress=missingIncludeSystem"
			"--suppress=*:*extern/mbed-os\*"
			"--template=🔥 warning: {id} ({severity}): {message}\\n    in {file}:{line}\\n{code}"
			# "--check-config"
		)
	endif(CPPCHECK)
endif(ENABLE_CODE_ANALYSIS)

# Set C/C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS TRUE)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS TRUE)

# Add custom flags
add_compile_options(
	-fdiagnostics-color=always
	$<$<COMPILE_LANGUAGE:CXX>:-Wno-register>
)

# Before all, set ROOT_DIR, MBED_OS_DIR
set(ROOT_DIR    ${CMAKE_CURRENT_LIST_DIR})
set(MBED_OS_DIR ${ROOT_DIR}/extern/mbed-os)
set(MCUBOOT_DIR ${ROOT_DIR}/extern/mcuboot/boot)

# Bootloader option
option(ENABLE_BOOTLOADER "Build LekaOS with bootloader" OFF)
message(STATUS "ENABLE_BOOTLOADER (Build LekaOS with bootloader) --> ${ENABLE_BOOTLOADER}")
if (ENABLE_BOOTLOADER)
	set(MBED_APP_FLAGS
		-DMBED_APP_START=0x08040000
		-DMBED_APP_SIZE=0x180000
	)
endif(ENABLE_BOOTLOADER)

# And include mbed-cmake.cmake
include(./mbed-cmake.cmake)

# Then configure name of the project
project(LekaOS LANGUAGES C CXX ASM)

# Generate compile commands database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

# For convenience you can define useful variables
set(OS_DIR      ${ROOT_DIR}/src)
set(LIBS_DIR    ${ROOT_DIR}/libs)
set(TESTS_DIR   ${ROOT_DIR}/tests)
set(SPIKES_DIR  ${ROOT_DIR}/spikes)
set(DRIVERS_DIR ${ROOT_DIR}/drivers)
set(TARGETS_DIR ${ROOT_DIR}/targets)
set(INCLUDE_DIR ${ROOT_DIR}/include)

# Add custom target subdirectory
set(AVAILABLE_CUSTOM_TARGETS DISCO_ORIGINAL LEKA_V1_0_DEV LEKA_V1_1_DEV LEKA_V1_2_DEV)
if (${TARGET_BOARD} IN_LIST AVAILABLE_CUSTOM_TARGETS)
	add_subdirectory(${TARGETS_DIR}/TARGET_${TARGET_BOARD})
endif()

# Add c++ support, include directories
include_directories(BEFORE SYSTEM
	${INCLUDE_DIR}/cxxsupport
	${MBED_OS_DIR}/platform/cxxsupport
)

include_directories(BEFORE
	${INCLUDE_DIR}
)

# Link libraries to all targets
add_subdirectory(${LIBS_DIR}/Utils)
add_subdirectory(${LIBS_DIR}/LogKit)
add_subdirectory(${LIBS_DIR}/HelloWorld)
add_subdirectory(${LIBS_DIR}/CircularBuffer)
add_subdirectory(${LIBS_DIR}/CriticalSection)

link_libraries(
	Utils
	LogKit
	HelloWorld
	CircularBuffer
	CriticalSection
)

# Add external libraries
add_subdirectory(${MCUBOOT_DIR})

# Add drivers & libraries
add_subdirectory(${DRIVERS_DIR})
add_subdirectory(${LIBS_DIR})

# Add spikes, functional tests
add_subdirectory(${SPIKES_DIR})
add_subdirectory(${TESTS_DIR}/functional)

# Add LekaOS
add_subdirectory(${OS_DIR})

# Finally print the mbed-cmake build report
mbed_cmake_print_build_report()
