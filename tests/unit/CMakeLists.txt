# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.18)

# Activate ccache if available
find_program(CCACHE "ccache")
if(CCACHE)
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE}")
endif(CCACHE)

message(STATUS "")

message(STATUS [==[                                                                                          ]==])
message(STATUS [==[      _          _          ____   _____   _    _       _ _   _______        _            ]==])
message(STATUS [==[     | |        | |        / __ \ / ____| | |  | |     (_) | |__   __|      | |           ]==])
message(STATUS [==[     | |     ___| | ____ _| |  | | (___   | |  | |_ __  _| |_   | | ___  ___| |_ ___      ]==])
message(STATUS [==[     | |    / _ \ |/ / _` | |  | |\___ \  | |  | | '_ \| | __|  | |/ _ \/ __| __/ __|     ]==])
message(STATUS [==[     | |___|  __/   < (_| | |__| |____) | | |__| | | | | | |_   | |  __/\__ \ |_\__ \     ]==])
message(STATUS [==[     |______\___|_|\_\__,_|\____/|_____/   \____/|_| |_|_|\__|  |_|\___||___/\__|___/     ]==])
message(STATUS [==[                                                                                          ]==])

message(STATUS "")

#
# Mark: - Project setup
#

project(LekaOSUnitTests LANGUAGES C CXX)

# Set -std=gnu++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Colored output for compilation
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	message(STATUS "GCC found! Adding -fdiagnostics-color=always compile option")
	add_compile_options("-fdiagnostics-color=always")
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	message(STATUS "Clang found! Adding -fcolor-diagnostics compile option")
	add_compile_options("-fcolor-diagnostics")
endif()

# Add code coverage
if (COVERAGE)
	if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
		message(WARNING "Non-debug build may result in misleading code coverage results.")
	endif()

	# Append coverage compiler flags
	set(COVERAGE_COMPILER_FLAGS "-g -O0 --coverage")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_COMPILER_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}")
endif(COVERAGE)

# Enable logging for tests
add_definitions(-DENABLE_LOG_DEBUG)

# Generate compile commands database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

#
# Mark: - Define directories
#

set(UNIT_TESTS_DIR     ${CMAKE_CURRENT_LIST_DIR})
set(ROOT_DIR           ${UNIT_TESTS_DIR}/../..)
set(EXTERN_MBED_OS_DIR ${ROOT_DIR}/extern/mbed-os)

set(LIBS_DIR      ${ROOT_DIR}/libs)
set(DRIVERS_DIR   ${ROOT_DIR}/drivers)
set(INCLUDE_DIR   ${ROOT_DIR}/include)

#
# Mark: - Add GoogleTest & GMock
#

configure_file(GoogleTest.in.cmake googletest-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND}
	-G "${CMAKE_GENERATOR}" .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
)

if(result)
	message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND}
	--build .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
)

if(result)
	message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Add googletest directly to our build. This defines the gtest and gtest_main targets.
add_subdirectory(
	${CMAKE_CURRENT_BINARY_DIR}/googletest-src
	${CMAKE_CURRENT_BINARY_DIR}/googletest-build
	EXCLUDE_FROM_ALL
)

#
# Mark: - Setup unit tests
#

enable_testing()
add_executable(LekaOSUnitTestsExec)

#
# Mark: - Define functions
#

add_custom_command(
	TARGET LekaOSUnitTestsExec
	PRE_BUILD
	COMMAND find . -name "*.gcda" -print0 | xargs -0 -r rm
)

# Define function to add sources to LekaOS Unit Tests
function(leka_unit_tests_sources UNIT_TESTS_SOURCES)
	set(_UNIT_TESTS_SOURCES ${UNIT_TESTS_SOURCES} ${ARGN})
	target_sources(LekaOSUnitTestsExec
		PRIVATE
			${_UNIT_TESTS_SOURCES}
	)
endfunction()

# Define function to register new unit tests for libraries
function(leka_register_unit_tests_for_library LIBRARY)
	add_subdirectory("${LIBS_DIR}/${LIBRARY}" "libs/${LIBRARY}")
	target_link_libraries(LekaOSUnitTestsExec
		PRIVATE
			${LIBRARY}
	)
endfunction()

# Define function to register new unit tests for drivers
function(leka_register_unit_tests_for_driver DRIVER)
	add_subdirectory("${DRIVERS_DIR}/${DRIVER}" "drivers/${DRIVER}")
	target_link_libraries(LekaOSUnitTestsExec
		PRIVATE
			${DRIVER}
	)
endfunction()

#
# Mark: - C++ Support, interfaces
#

include_directories(BEFORE SYSTEM
	${EXTERN_MBED_OS_DIR}/UNITTESTS/target_h/platform/cxxsupport
	${EXTERN_MBED_OS_DIR}/platform/cxxsupport
	${INCLUDE_DIR}/cxxsupport
	${INCLUDE_DIR}
)

#
# Mark: - Headers
#

add_subdirectory(./headers)

#
# Mark: - Add fakes, stubs & mocks
#

add_subdirectory(./stubs)
add_subdirectory(./mocks)

#
# Mark: - Register tests for util libraries
#

leka_register_unit_tests_for_library(Utils)
leka_register_unit_tests_for_library(LogKit)
leka_register_unit_tests_for_library(CriticalSection)

#
# Mark: - Link util libraries for remaining tests
#

link_libraries(
	Utils
	LogKit
	CriticalSection
)

#
# Mark: - Register tests for drivers and libraries
#

add_subdirectory(template)

# Register drivers
leka_register_unit_tests_for_driver(CoreFs)
leka_register_unit_tests_for_driver(LKCoreLL)
leka_register_unit_tests_for_driver(LKCoreSTM32Hal)
leka_register_unit_tests_for_driver(CoreVideo)
leka_register_unit_tests_for_driver(LKCoreLightSensor)
leka_register_unit_tests_for_driver(LKCoreMicrophone)
leka_register_unit_tests_for_driver(CoreBattery)

leka_register_unit_tests_for_driver(CoreHTS)
leka_register_unit_tests_for_driver(CoreI2C)
leka_register_unit_tests_for_driver(CorePwm)
leka_register_unit_tests_for_driver(CoreQSPI)
leka_register_unit_tests_for_driver(CoreMotor)
leka_register_unit_tests_for_driver(CoreFlashMemory)
leka_register_unit_tests_for_driver(CoreBufferedSerial)

# Register libraries
leka_register_unit_tests_for_library(LKAnimationKit)
leka_register_unit_tests_for_library(LKCalculatorKit)
leka_register_unit_tests_for_library(ContainerKit)
leka_register_unit_tests_for_library(FileSystemKit)
leka_register_unit_tests_for_library(FirmwareKit)
leka_register_unit_tests_for_library(IOKit)

#
# Mark: - Finish up
#

target_link_libraries(LekaOSUnitTestsExec
	PRIVATE
		gtest_main
		gmock_main
		mbed-os
		mocks
		stubs
)

add_test(NAME LekaOSUnitTests COMMAND LekaOSUnitTestsExec)
