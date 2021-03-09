# mbed-cmake requires CMake >= 3.12 for the new FindPython3 module
cmake_policy(VERSION 3.12)

message(STATUS [==[                                                                   ]==])
message(STATUS [==[      _          _          ____   _____                  _        ]==])
message(STATUS [==[     | |        | |        / __ \ / ____|   ___          | |       ]==])
message(STATUS [==[     | |     ___| | ____ _| |  | | (___    ( _ )     __ _| |       ]==])
message(STATUS [==[     | |    / _ \ |/ / _` | |  | |\___ \   / _ \/\  / _` | |       ]==])
message(STATUS [==[     | |___|  __/   < (_| | |__| |____) | | (_>  < | (_| | |_      ]==])
message(STATUS [==[     |______\___|_|\_\__,_|\____/|_____/   \___/\/  \__,_|_(_)     ]==])
message(STATUS [==[                                                                   ]==])

message(STATUS "")

set(MBED_CMAKE_VERSION 1.3.0)
message(STATUS "mbed-cmake version ${MBED_CMAKE_VERSION}, running on CMake ${CMAKE_VERSION}")

set(MBED_CMAKE_ROOT_DIR ${ROOT_DIR}/cmake)
set(MBED_CMAKE_SCRIPTS_DIR ${MBED_CMAKE_ROOT_DIR}/scripts)
set(MBED_CMAKE_CONFIG_DIR ${ROOT_DIR}/_build/cmake_config/${TARGET_BOARD})

list(APPEND CMAKE_MODULE_PATH ${MBED_CMAKE_ROOT_DIR})
list(APPEND CMAKE_MODULE_PATH ${MBED_CMAKE_ROOT_DIR}/utils)

# Fix error: The C Compiler is not able to compile a simple test program
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

include(CheckTypeSize)
include(Shorthand)
include(Utils)
include(CheckPythonPackage)
include(FindPackageHandleStandardArgs)

# build configuration
# -------------------------------------------------------------
# default to RelWithDebInfo to match Mbed OS default
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
		"Type of build, options are: Debug, RelWithDebInfo, Release" FORCE)
endif()

# Allowed build types are Debug and Release
set(CMAKE_CONFIGURATION_TYPES Debug RelWithDebInfo Release)

# check configuration files
# -------------------------------------------------------------

set(MBED_CMAKE_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

# relative to CMakeLists.txt inside mbed-cmake
if(NOT DEFINED MBED_CMAKE_GENERATED_CONFIG_RELPATH)
	set(MBED_CMAKE_GENERATED_CONFIG_RELPATH ${MBED_CMAKE_CONFIG_DIR})
endif()
set(MBED_CMAKE_GENERATED_CONFIG_PATH ${MBED_CMAKE_CONFIG_DIR})

if(NOT DEFINED MBED_CMAKE_CONFIG_HEADERS_PATH)
	set(MBED_CMAKE_CONFIG_HEADERS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/config-headers)
endif()

if(NOT EXISTS ${MBED_CMAKE_GENERATED_CONFIG_PATH}/cmake/MbedOSConfig.cmake)
	message(FATAL_ERROR "Mbed config files and headers do not exist!  You need to run mbed-cmake/configure_for_target.py from the top source dir!")
endif()

# unit test config
# -------------------------------------------------------------
option(MBED_UNITTESTS "If true, compile for the local system and run unit tests" FALSE)

# load compilers and flags
# -------------------------------------------------------------

# read flags from generated configuration file
include(${MBED_CMAKE_GENERATED_CONFIG_PATH}/cmake/MbedOSConfig.cmake)

# load toolchain
if(MBED_UNITTESTS)
	message(STATUS "Unit tests enabled, not loading mbed-cmake toolchain file")
elseif("${MBED_TOOLCHAIN_NAME}" STREQUAL "ARMC6")

	if(${CMAKE_VERSION} VERSION_LESS 3.15.0)
		message(FATAL_ERROR "CMake >= 3.15.0 is required for Arm Compiler support")
	endif()

	include(ToolchainArmClangDefine)
elseif("${MBED_TOOLCHAIN_NAME}" STREQUAL "GCC_ARM")
	include(ToolchainGCCArmDefine)
else()
	message(FATAL_ERROR "Unknown toolchain \"${MBED_TOOLCHAIN_NAME}\"")
endif()

# search for and load compilers
enable_language(C CXX ASM)

if(NOT MBED_UNITTESTS)
	# Set config-specific flags
	# Note: unlike CMAKE_C_FLAGS etc, we need to set these AFTER the project() call in order to override CMake's default values for these flags.
	# CMAKE_C_FLAGS_INIT etc need to be set BEFORE the project() call so that it's used when CMake scans compiler properties.
	foreach(BUILD_TYPE RELWITHDEBINFO DEBUG RELEASE)
		list_to_space_separated(CMAKE_C_FLAGS_${BUILD_TYPE} ${MCU_COMPILE_OPTIONS_${BUILD_TYPE}})
		list_to_space_separated(CMAKE_CXX_FLAGS_${BUILD_TYPE} ${MCU_COMPILE_OPTIONS_${BUILD_TYPE}})
		list_to_space_separated(CMAKE_ASM_FLAGS_${BUILD_TYPE} ${MCU_COMPILE_OPTIONS_${BUILD_TYPE}})
	endforeach()
endif()

# find python (used for memap and several upload methods)
# -------------------------------------------------------------

find_package(Python3 COMPONENTS Interpreter)

# Configure unit tests
# -------------------------------------------------------------

if(MBED_UNITTESTS)
	# Build internal GTest.
	# We use an internal GTest because hardly any platform has a complete package available
	# for it for some reason.
	# add_subdirectory(${MBED_CMAKE_SOURCE_DIR}/gtest-external-project)
	# include(GoogleTest)
	# enable_testing()
endif()

# load the Mbed CMake functions
# -------------------------------------------------------------

if(MBED_UNITTESTS)
	include(AddMbedUnitTestsExecutable)
else()
	include(AddMbedExecutable)
endif()

# Configure upload methods
# -------------------------------------------------------------

if(NOT MBED_UNITTESTS)
	set(CMAKE_EXECUTABLE_SUFFIX .elf)

	# find upload tools
	# find_package(OpenOCD)
	# find_package(JLINK)
	# include(SetUploadMethod)
endif()

# add Mbed OS source
# -------------------------------------------------------------

set(MBED_CMAKE_CONFIG_HEADERS_PATH ${MBED_CMAKE_GENERATED_CONFIG_PATH}/config-headers)
add_subdirectory(${MBED_OS_DIR}) #first get Mbed standard library

# build report
# -------------------------------------------------------------
function(mbed_cmake_print_build_report)

	message(STATUS "---- Completed configuration of ${PROJECT_NAME} ----")

	# build type
	message(STATUS ">> Current Build Type: ${CMAKE_BUILD_TYPE}")

	# build flags
	string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UCASE)
	message(STATUS ">> C   Compile Flags (Global): ${CMAKE_C_FLAGS}")
	message(STATUS ">> C   Compile Flags (For ${CMAKE_BUILD_TYPE}): ${CMAKE_C_FLAGS_${CMAKE_BUILD_TYPE_UCASE}}")
	message(STATUS ">> CXX Compile Flags (Global): ${CMAKE_CXX_FLAGS}")
	message(STATUS ">> CXX Compile Flags (For ${CMAKE_BUILD_TYPE}): ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE_UCASE}}")
	message(STATUS ">> Linker Flags: ${CMAKE_EXE_LINKER_FLAGS}")

endfunction(mbed_cmake_print_build_report)
