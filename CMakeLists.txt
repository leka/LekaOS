# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.21)

# Activate ccache if available
find_program(CCACHE "ccache")
if(CCACHE)
	set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE}")
	set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE}")
endif(CCACHE)

# Activate code analysis
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

# Activate LogKit
option(ENABLE_LOG_DEBUG "Enable LogKit output" OFF)
message(STATUS "ENABLE_LOG_DEBUG --> ${ENABLE_LOG_DEBUG}")
if(ENABLE_LOG_DEBUG)
	add_definitions(-DENABLE_LOG_DEBUG)
endif(ENABLE_LOG_DEBUG)

# Set C/C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS TRUE)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS TRUE)

# Add custom flags
add_compile_options(
	-fdiagnostics-color=always
)

# Before all, set ROOT_DIR, MBED_OS_DIR
set(ROOT_DIR         ${CMAKE_CURRENT_LIST_DIR})
set(MBED_PATH        ${ROOT_DIR}/extern/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${ROOT_DIR}/.mbedbuild CACHE INTERNAL "")
set(MCUBOOT_DIR      ${ROOT_DIR}/extern/mcuboot/boot)
set(MBED_HTTP_DIR    ${ROOT_DIR}/extern/mbed-http)

# Bootloader option
option(BUILD_TARGETS_TO_USE_WITH_BOOTLOADER "Build targets with padding to use with bootloader" OFF)
message(STATUS "Build targets with padding to use with bootloader --> ${BUILD_TARGETS_TO_USE_WITH_BOOTLOADER}")
if (BUILD_TARGETS_TO_USE_WITH_BOOTLOADER)
	set(MBED_APP_FLAGS
		-DMBED_APP_START=0x08041000
		-DMBED_APP_SIZE=0x17E000
	)
endif(BUILD_TARGETS_TO_USE_WITH_BOOTLOADER)

# Include mbed os cmake logic
include(${MBED_PATH}/tools/cmake/app.cmake)

# Then configure name of the project
project(LekaOS LANGUAGES C CXX ASM)

# Generate compile commands database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if(CMAKE_EXPORT_COMPILE_COMMANDS)
	set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

# For convenience you can define useful variables
set(OS_DIR         ${ROOT_DIR}/app/os)
set(LIBS_DIR       ${ROOT_DIR}/libs)
set(TESTS_DIR      ${ROOT_DIR}/tests)
set(SPIKES_DIR     ${ROOT_DIR}/spikes)
set(DRIVERS_DIR    ${ROOT_DIR}/drivers)
set(TARGETS_DIR    ${ROOT_DIR}/targets)
set(INCLUDE_DIR    ${ROOT_DIR}/include)
set(BOOTLOADER_DIR ${ROOT_DIR}/app/bootloader)

# Add mbed-os
add_subdirectory(${MBED_PATH})

# Add custom target subdirectory
set(AVAILABLE_CUSTOM_TARGETS LEKA_DISCO LEKA_V1_2_DEV)
if (${TARGET_BOARD} IN_LIST AVAILABLE_CUSTOM_TARGETS)
	add_subdirectory(${TARGETS_DIR}/${TARGET_BOARD})
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
add_subdirectory(${LIBS_DIR}/CriticalSection)

link_libraries(
	Utils
	LogKit
	HelloWorld
	CriticalSection
)

# Add external libraries
add_compile_definitions(MCUBOOT_BOOTLOADER_SIZE=0x40000)
add_compile_definitions(MCUBOOT_SCRATCH_SIZE=0x40000)
add_compile_definitions(MCUBOOT_SCRATCH_START_ADDR=0x081C0000)
add_compile_definitions(MCUBOOT_RSA_SIGNATURE_LENGTH=2048)
add_compile_definitions(MCUBOOT_SLOT_SIZE=0x200000-MCUBOOT_BOOTLOADER_SIZE-MCUBOOT_SCRATCH_SIZE)

add_subdirectory(${MCUBOOT_DIR})
add_subdirectory(${MBED_HTTP_DIR})

# Add drivers & libraries
add_subdirectory(${DRIVERS_DIR})
add_subdirectory(${LIBS_DIR})

# Add spikes, functional tests
add_subdirectory(${SPIKES_DIR})
add_subdirectory(${TESTS_DIR}/functional)

# Add bootloader
add_subdirectory(${BOOTLOADER_DIR})

# Add LekaOS
add_subdirectory(${OS_DIR})

option(VERBOSE_BUILD "Have a verbose build process")
if(VERBOSE_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()
