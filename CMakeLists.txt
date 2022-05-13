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

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# Add custom flags
add_compile_options(
	$<$<COMPILE_LANGUAGE:CXX>:-Wno-register>
	$<$<COMPILE_LANGUAGE:CXX>:-Wno-volatile>
	-fdiagnostics-color=always
)

# Bootloader
option(BUILD_TARGETS_TO_USE_WITH_BOOTLOADER "Build targets with padding to use with bootloader" OFF)
if (BUILD_TARGETS_TO_USE_WITH_BOOTLOADER)
	set(MBED_APP_FLAGS
		-DMBED_APP_START=0x08041000
		-DMBED_APP_SIZE=0x17E000
	)
endif(BUILD_TARGETS_TO_USE_WITH_BOOTLOADER)

# Logger
option(ENABLE_LOG_DEBUG "Enable LogKit output" OFF)
if(ENABLE_LOG_DEBUG)
	add_definitions(-DENABLE_LOG_DEBUG)
endif(ENABLE_LOG_DEBUG)

# SYSTEM Stats
option(ENABLE_SYSTEM_STATS "Enable SYSTEM stats" OFF)
if(ENABLE_LOG_DEBUG AND ENABLE_SYSTEM_STATS)
	add_definitions(-DMBED_CPU_STATS_ENABLED)
	add_definitions(-DMBED_STACK_STATS_ENABLED)
	add_definitions(-DMBED_HEAP_STATS_ENABLED)
endif(ENABLE_LOG_DEBUG AND ENABLE_SYSTEM_STATS)

#
# MARK: - Directories
#

# Before all
set(ROOT_DIR      ${CMAKE_CURRENT_LIST_DIR})

# extern
set(MBED_OS_DIR   ${ROOT_DIR}/extern/mbed-os)
set(MCUBOOT_DIR   ${ROOT_DIR}/extern/mcuboot/boot)
set(MBED_HTTP_DIR ${ROOT_DIR}/extern/mbed-http)

# includes
set(LIBS_DIR    ${ROOT_DIR}/libs)
set(DRIVERS_DIR ${ROOT_DIR}/drivers)
set(INCLUDE_DIR ${ROOT_DIR}/include)
set(TARGETS_DIR ${ROOT_DIR}/targets)

# app
set(OS_DIR         ${ROOT_DIR}/app/os)
set(BOOTLOADER_DIR ${ROOT_DIR}/app/bootloader)

# spikes, functional tests
set(SPIKES_DIR           ${ROOT_DIR}/spikes)
set(FUNCTIONAL_TESTS_DIR ${ROOT_DIR}/tests/functional)

#
# MARK: - LekaOS Project
#

# And include mbed-cmake.cmake
include(${ROOT_DIR}/cmake/mbed-cmake.cmake)

project(LekaOS LANGUAGES C CXX ASM)

# Add custom target subdirectory
set(AVAILABLE_CUSTOM_TARGETS LEKA_DISCO LEKA_V1_2_DEV)
if (${TARGET_BOARD} IN_LIST AVAILABLE_CUSTOM_TARGETS)
	message(STATUS "Add subdirectory for hardware target ${TARGET_BOARD}")
	add_subdirectory(${TARGETS_DIR}/TARGET_${TARGET_BOARD})
else()
	message(FATAL_ERROR "Target ${TARGET_BOARD} is not available.")
endif()

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

add_subdirectory(${MCUBOOT_DIR})
add_subdirectory(${MBED_HTTP_DIR})

# Add drivers & libraries
add_subdirectory(${DRIVERS_DIR})
add_subdirectory(${LIBS_DIR})

# Add spikes, functional tests
add_subdirectory(${SPIKES_DIR})
add_subdirectory(${FUNCTIONAL_TESTS_DIR})

# Add bootloader
add_subdirectory(${BOOTLOADER_DIR})

# Add LekaOS
add_subdirectory(${OS_DIR})

# Finally print the mbed-cmake build report
mbed_cmake_print_build_report()

#
# MARK: - Misc. options
#

option(VERBOSE_BUILD "Have a verbose build process")
if(VERBOSE_BUILD)
	set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

# Print options
message(STATUS "")
message(STATUS "CMAKE_EXPORT_COMPILE_COMMANDS --> ${CMAKE_EXPORT_COMPILE_COMMANDS}")
message(STATUS "ENABLE_LOG_DEBUG              --> ${ENABLE_LOG_DEBUG}")
message(STATUS "ENABLE_SYSTEM_STATS           --> ${ENABLE_SYSTEM_STATS}")
message(STATUS "BUILD_TARGETS_FOR_BOOTLOADER  --> ${BUILD_TARGETS_TO_USE_WITH_BOOTLOADER}")
message(STATUS "CMAKE_VERBOSE_MAKEFILE        --> ${VERBOSE_BUILD}")
message(STATUS "")
