# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.21)

#
# MARK: - External tools
#
find_program(CCACHE "ccache")

if(CCACHE)
	set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE}")
	set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE}")
endif(CCACHE)

# Compile commands database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if(CMAKE_EXPORT_COMPILE_COMMANDS)
	set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

#
# MARK: - Shiny output
#
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

#
# MARK: - Compilation flags/options
#

# Set C/C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS TRUE)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Colored output for compilation
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	message(STATUS "GCC found! Adding -fdiagnostics-color=always compile option")
	add_compile_options("-fdiagnostics-color=always")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	message(STATUS "Clang found! Adding -fcolor-diagnostics compile option")
	add_compile_options("-fcolor-diagnostics")
else()
	message(FATAL_ERROR "No suitable compiler found...")
endif()

# Add code coverage
if(COVERAGE)
	if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
		message(FATAL_ERROR "Non-debug build may result in misleading code coverage results.")
	endif()

	add_compile_options(-g -O0 --coverage -fno-exceptions -fno-inline)

	add_link_options(--coverage)
endif(COVERAGE)

# Add address sanitizer
if(SANITIZERS)
	if(NOT COVERAGE)
		message(FATAL_ERROR "Address Sanitizer only works when coverage is enabled.")
	endif()

	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		add_compile_options(
			-fno-omit-frame-pointer
			-fno-optimize-sibling-calls
			-fsanitize=address
			-fsanitize=pointer-compare
			-fsanitize=pointer-subtract
			-fsanitize=leak
			-fsanitize=undefined
			-fsanitize-address-use-after-scope
		)
	elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
		add_compile_options(
			-fno-omit-frame-pointer
			-fno-optimize-sibling-calls
			-fsanitize=address
			-fsanitize=pointer-compare
			-fsanitize=pointer-subtract
			-fsanitize=undefined
			-fsanitize-address-use-after-scope
		)
	else()
		message(FATAL_ERROR "Compiler not recognized")
	endif()

	add_link_options(-fsanitize=address -fsanitize=undefined)

	if(CI_UT_OPTIMIZATION_LEVEL)
		add_compile_options(${CI_UT_OPTIMIZATION_LEVEL})
	endif(CI_UT_OPTIMIZATION_LEVEL)
endif(SANITIZERS)

# Enable logging for tests
add_definitions(-DENABLE_LOG_DEBUG)

#
# MARK: - Directories
#
set(UNIT_TESTS_DIR ${CMAKE_CURRENT_LIST_DIR})
set(ROOT_DIR ${UNIT_TESTS_DIR}/../..)
set(EXTERN_MBED_OS_DIR ${ROOT_DIR}/extern/mbed-os)

set(LIBS_DIR ${ROOT_DIR}/libs)
set(DRIVERS_DIR ${ROOT_DIR}/drivers)
set(INCLUDE_DIR ${ROOT_DIR}/include)

#
# MARK: - External tools
#
list(APPEND CMAKE_MODULE_PATH ${ROOT_DIR}/cmake/utils)

include(Utils)

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
# Mark: - Define macros, functions, custom commands
#
add_custom_command(
	TARGET LekaOSUnitTestsExec
	PRE_BUILD
	COMMAND find . -name "*.gcda" -o -name "*.gcov" | xargs -r rm -rf
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
	${EXTERN_MBED_OS_DIR}/platform/cxxsupport
	${INCLUDE_DIR}
)

#
# Mark: - Headers
#
add_subdirectory(${UNIT_TESTS_DIR}/headers)

#
# Mark: - Add fakes, stubs & mocks
#
add_subdirectory(${UNIT_TESTS_DIR}/stubs)
add_subdirectory(${UNIT_TESTS_DIR}/mocks)

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
leka_register_unit_tests_for_driver(CoreBattery)
leka_register_unit_tests_for_driver(CoreBufferedSerial)
leka_register_unit_tests_for_driver(CoreDAC)
leka_register_unit_tests_for_driver(CoreEventFlags)
leka_register_unit_tests_for_driver(CoreEventQueue)
leka_register_unit_tests_for_driver(CoreFlashMemory)
leka_register_unit_tests_for_driver(CoreHTS)
leka_register_unit_tests_for_driver(CoreI2C)
leka_register_unit_tests_for_driver(CoreInterruptIn)
leka_register_unit_tests_for_driver(CoreIMU)
leka_register_unit_tests_for_driver(CoreIOExpander)
leka_register_unit_tests_for_driver(CoreLED)
leka_register_unit_tests_for_driver(CoreLightSensor)
leka_register_unit_tests_for_driver(CoreLL)
leka_register_unit_tests_for_driver(CoreMCU)
leka_register_unit_tests_for_driver(CoreMicrophone)
leka_register_unit_tests_for_driver(CoreMotor)
leka_register_unit_tests_for_driver(CoreMutex)
leka_register_unit_tests_for_driver(CorePwm)
leka_register_unit_tests_for_driver(CoreQDAC)
leka_register_unit_tests_for_driver(CoreQSPI)
leka_register_unit_tests_for_driver(CoreRFIDReader)
leka_register_unit_tests_for_driver(CoreSPI)
leka_register_unit_tests_for_driver(CoreSTM32Hal)
leka_register_unit_tests_for_driver(CoreTicker)
leka_register_unit_tests_for_driver(CoreTouchSensor)
leka_register_unit_tests_for_driver(CoreVideo)

# Register libraries
leka_register_unit_tests_for_library(ActivityKit)
leka_register_unit_tests_for_library(BatteryKit)
leka_register_unit_tests_for_library(BehaviorKit)
leka_register_unit_tests_for_library(BLEKit)
leka_register_unit_tests_for_library(ColorKit)
leka_register_unit_tests_for_library(CommandKit)
leka_register_unit_tests_for_library(ConfigKit)
leka_register_unit_tests_for_library(ContainerKit)
leka_register_unit_tests_for_library(EventLoopKit)
leka_register_unit_tests_for_library(FileManagerKit)
leka_register_unit_tests_for_library(FirmwareKit)
leka_register_unit_tests_for_library(WebKit)
leka_register_unit_tests_for_library(IMUKit)
leka_register_unit_tests_for_library(IOKit)
leka_register_unit_tests_for_library(LedKit)
leka_register_unit_tests_for_library(MotionKit)
leka_register_unit_tests_for_library(ReinforcerKit)
leka_register_unit_tests_for_library(RobotKit)
leka_register_unit_tests_for_library(RFIDKit)
leka_register_unit_tests_for_library(SerialNumberKit)
leka_register_unit_tests_for_library(TouchSensorKit)
leka_register_unit_tests_for_library(UIAnimationKit)
leka_register_unit_tests_for_library(VideoKit)

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

#
# MARK: - Options
#
option(VERBOSE_BUILD "Have a verbose build process")

if(VERBOSE_BUILD)
	set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

print_compilers_flags_options_definitions()

message(STATUS ">> ")
message(STATUS ">> External definitions")
message(STATUS ">> ")
message(STATUS "")

message(STATUS "UT LITE                  --> ${UT_LITE}")
message(STATUS "CODE_COVERAGE            --> ${COVERAGE}")
message(STATUS "SANITIZERS               --> ${SANITIZERS}")

if(NOT ${CI_UT_OPTIMIZATION_LEVEL} STREQUAL "")
	message(STATUS "CI_UT_OPTIMIZATION_LEVEL --> ${CI_UT_OPTIMIZATION_LEVEL}")
else()
	message(STATUS "CI_UT_OPTIMIZATION_LEVEL --> (not used)")
endif(NOT ${CI_UT_OPTIMIZATION_LEVEL} STREQUAL "")

message(STATUS "")
