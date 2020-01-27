cmake_minimum_required(VERSION 3.0.2)

set(PROJECT_NAME test_unit)
set(LIB_NAME MbedOS)

project(${PROJECT_NAME})

# Setup c++ standard
macro(use_cxx14)
    if (CMAKE_VERSION VERSION_LESS 3.1)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")
        endif()
    else()
        set(CMAKE_CXX_STANDARD 14)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
    endif()
endmacro()

use_cxx14()

####################
# DEFINE DIRECTORIES
####################

get_filename_component(PROJECT_DOT_MBED_FILE "../../.mbed" REALPATH)
get_filename_component(PROJECT_ROOT_DIR ${PROJECT_DOT_MBED_FILE} DIRECTORY)

set(PROJECT_LIB_DIR "${PROJECT_ROOT_DIR}/lib")
set(PROJECT_UNITTESTS_DIR "${PROJECT_ROOT_DIR}/test/unit")

set(MBED_OS_DIR "${PROJECT_ROOT_DIR}/libvendor/mbed-os")
set(MBED_OS_UNITTESTS_DIR "${MBED_OS_DIR}/UNITTESTS")

message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "PROJECT_ROOT_DIR: ${PROJECT_ROOT_DIR}")

message(STATUS "PROJECT_LIB_DIR: ${PROJECT_LIB_DIR}")
message(STATUS "PROJECT_UNITTESTS_DIR: ${PROJECT_UNITTESTS_DIR}")

message(STATUS "MBED_OS_DIR: ${MBED_OS_DIR}")
message(STATUS "MBED_OS_UNITTESTS_DIR: ${MBED_OS_UNITTESTS_DIR}")

function (prettyPrintList)
    set (list ${ARGN})
    list(LENGTH list list_length)
    if (${list_length} GREATER 0)
      foreach(item ${list})
        message("\t- ${item}")
      endforeach(item)
    endif ()
endfunction(prettyPrintList)

####################
# GTEST
####################

# Download and unpack googletest at configure time
configure_file(googletest-CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "CMake failed for google test: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "Build failed for google test: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main/gmock/gmock_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later.
target_include_directories(gmock_main SYSTEM BEFORE INTERFACE
  "$<BUILD_INTERFACE:${gtest_SOURCE_DIR}/include>"
  "$<BUILD_INTERFACE:${gmock_SOURCE_DIR}/include>")

####################
# TESTING
####################

include(CTest)

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
  "${CMAKE_BINARY_DIR}/Testing"
)

####################
# CODE COVERAGE SETUP
####################

if (COVERAGE)

  if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(WARNING "Non-debug build may result misleading code coverage results.")
  endif()

  # Append coverage compiler flags
  set(COVERAGE_COMPILER_FLAGS "-g -O0 --coverage")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_COMPILER_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}")

endif(COVERAGE)

# set(GENERAL_FLAGS "--coverage -fprofile-arcs -ftest-coverage -DUNITTEST -D__unix__ -DDEVICE_WATCHDOG -DMBED_WDOG_ASSERT=1 -pthread -DEQUEUE_PLATFORM_POSIX -DDEVICE_SERIAL=1 -DDEVICE_INTERRUPTIN=1 -DMBED_CONF_NSAPI_DEFAULT_CELLULAR_APN=NULL -DMBED_CONF_NSAPI_DEFAULT_CELLULAR_USERNAME=NULL -DMBED_CONF_NSAPI_DEFAULT_CELLULAR_PASSWORD=NULL -DMBED_CONF_NSAPI_DEFAULT_CELLULAR_PLMN=NULL -DMBED_CONF_NSAPI_DEFAULT_CELLULAR_SIM_PIN=NULL -DMDMTXD=NC -DMDMRXD=NC -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200 -DMBED_CONF_CELLULAR_DEBUG_AT=true -DOS_STACK_SIZE=2048 -DMDMRTS=PTC0 -DMDMCTS=PTC1 -DMDMTXD=NC -DMDMRXD=NC -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200 -DCELLULAR_DEVICE=myCellularDevice -DDEVICE_SERIAL_FC=1 -DMBED_CONF_CELLULAR_CONTROL_PLANE_OPT=0 -DMDMRTS=PTC0 -DMDMCTS=PTC1 -DMDMTXD=NC -DMDMRXD=NC -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200 -DCELLULAR_DEVICE=myCellularDevice -DDEVICE_SERIAL_FC=1 -DMDMRTS=PTC0 -DMDMCTS=PTC1 -DMDMTXD=NC -DMDMRXD=NC -DMBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE=115200 -DCELLULAR_DEVICE=myCellularDevice -DDEVICE_SERIAL_FC=1 -DMBED_CONF_LORA_ADR_ON=true -DMBED_CONF_LORA_PUBLIC_NETWORK=true -DMBED_CONF_LORA_NB_TRIALS=2 -DMBED_CONF_LORA_DOWNLINK_PREAMBLE_LENGTH=5 -DMBED_CONF_LORA_DUTY_CYCLE_ON=true -DMBED_CONF_LORA_MAX_SYS_RX_ERROR=10 -DMBED_CONF_LORA_NWKSKEY=\"{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}\" -DMBED_CONF_LORA_APPSKEY=\"{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}\" -DMBED_CONF_LORA_DEVICE_ADDRESS=\"0x00000000\" -DMBED_CONF_LORA_TX_MAX_SIZE=255 -DMBED_CONF_LORA_UPLINK_PREAMBLE_LENGTH=8 -DMBED_CONF_LORA_DUTY_CYCLE_ON_JOIN=true -DMBED_CONF_LORA_WAKEUP_TIME=5 -DMBED_CONF_LORA_FSB_MASK=\"{0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00FF}\" -DMBED_CONF_LORA_FSB_MASK_CHINA=\"{0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00FF}\" -DMBED_CONF_LORA_FSB_MASK=\"{0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00FF}\" -DMBED_CONF_LORA_PHY=\"EU868\" -DMBED_CONF_LORA_OVER_THE_AIR_ACTIVATION=true -DMBED_CONF_LORA_DEVICE_EUI=\"{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}\" -DMBED_CONF_LORA_AUTOMATIC_UPLINK_MESSAGE=true -DMBED_CONF_LORA_APPLICATION_EUI=\"{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}\" -DMBED_CONF_LORA_APPLICATION_KEY=\"{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}\" -DNDEBUG=1")

####################
# UNIT TESTS
####################

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUNITTEST ${GENERAL_FLAGS}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNITTEST ${GENERAL_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUNITTEST")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNITTEST")

add_definitions(-DUNITTEST -D__unix__ -pthread)

set(WWW ../../libvendor/mbed-os/UNITTESTS)

# Set include dirs.
set(unittest-includes-base
  "${WWW}/target_h"
  "${WWW}/target_h/events"
  "${WWW}/target_h/events/equeue"
  "${WWW}/target_h/platform"
  "${WWW}/target_h/platform/cxxsupport"
  "${WWW}/target_h/drivers"
  "${WWW}/stubs"
  "${MBED_OS_DIR}"
  "${MBED_OS_DIR}/features"
  "${MBED_OS_DIR}/features/netsocket"
  "${MBED_OS_DIR}/platform"
  "${MBED_OS_DIR}/drivers"
  "${MBED_OS_DIR}/hal"
  "${MBED_OS_DIR}/events"
  "${MBED_OS_DIR}/events/source"
  "${MBED_OS_DIR}/events/internal"
  "${MBED_OS_DIR}/rtos"
  "${MBED_OS_DIR}/rtos/TARGET_CORTEX"
  "${MBED_OS_DIR}/rtos/TARGET_CORTEX/rtx5/Include"
  "${MBED_OS_DIR}/cmsis"
  "${MBED_OS_DIR}/features/frameworks"
  "${MBED_OS_DIR}/features/frameworks/mbed-trace"
  "${MBED_OS_DIR}/features/frameworks/nanostack-libservice"
  "${MBED_OS_DIR}/features/frameworks/nanostack-libservice/mbed-client-libservice"
  "${MBED_OS_DIR}/features/filesystem/fat"
  "${MBED_OS_DIR}/features/filesystem/fat/ChaN"
  "${MBED_OS_DIR}/features/filesystem/bd"
  "${MBED_OS_DIR}/features/filesystem/"
  "${MBED_OS_DIR}/features/filesystem/littlefs"
  "${MBED_OS_DIR}/features/filesystem/littlefs/littlefs"
  "${MBED_OS_DIR}/features/cellular/framework/API"
  "${MBED_OS_DIR}/features/cellular/framework/AT"
  "${MBED_OS_DIR}/features/cellular/framework/device"
  "${MBED_OS_DIR}/features/cellular/framework"
  "${MBED_OS_DIR}/features/cellular/framework/common"
  "${MBED_OS_DIR}/features/lorawan"
  "${MBED_OS_DIR}/features/lorawan/lorastack"
  "${MBED_OS_DIR}/features/lorawan/lorastack/mac"
  "${MBED_OS_DIR}/features/lorawan/lorastack/phy"
  "${MBED_OS_DIR}/features/lorawan/system"
  "${MBED_OS_DIR}/features/mbedtls"
  "${MBED_OS_DIR}/features/mbedtls/inc"
  "${MBED_OS_DIR}/features/mbedtls/mbed-crypto/inc"
)

# Create a list for test suites.
set(TEST_SUITES)

# Get all matched tests.
file(GLOB_RECURSE UNITTEST_FILE_LIST
  "unittest.cmake"
)

if ("${UNITTEST_FILE_LIST}" STREQUAL "")
  message(FATAL_ERROR "No tests found. Exiting...")
else()
  message(STATUS "UNITTEST_FILE_LIST")
  prettyPrintList(${UNITTEST_FILE_LIST})
endif()

# Create unit test targets
foreach(testfile ${UNITTEST_FILE_LIST})
  ####################
  # DEFINE TARGETS
  ####################

  # Init file lists.
  set(unittest-includes ${unittest-includes-base})
  set(unittest-sources)
  set(unittest-test-sources)

  # Get source files
  include("${testfile}")

  get_filename_component(TEST_SUITE_DIR ${testfile} DIRECTORY)

  file(RELATIVE_PATH
       TEST_SUITE_NAME # output
       ${PROJECT_ROOT_DIR} # root
       ${TEST_SUITE_DIR} #abs dirpath
  )

  string(REGEX REPLACE "/|\\\\" "-" TEST_SUITE_NAME ${TEST_SUITE_NAME})

  # message(STATUS "test suite name ${TEST_SUITE_NAME}")

  set(TEST_SUITES ${TEST_SUITES} ${TEST_SUITE_NAME})

  set(LIBS_TO_BE_LINKED gmock_main)

  # Build directories list
  set(BUILD_DIRECTORIES)

  set(UNITTEST_INCLUDES_REALPATH)

  foreach(include ${unittest-includes})
    get_filename_component(include_realpath "${include}" REALPATH)
    string(REGEX REPLACE  "mbed-os/mbed-os" "mbed-os" include_realpath ${include_realpath})
    list(APPEND UNITTEST_INCLUDES_REALPATH ${include_realpath})
  endforeach(include)

  set(UNITTEST_SOURCES_REALPATH)

  foreach(source ${unittest-sources})
    get_filename_component(source_realpath "${PROJECT_LIB_DIR}/${source}" REALPATH)
    # string(REGEX REPLACE  "mbed-os/mbed-os" "mbed-os" source_realpath ${source_realpath})
    # string(REGEX REPLACE  "stubs" "UNITTESTS/stubs" source_realpath ${source_realpath})
    list(APPEND UNITTEST_SOURCES_REALPATH ${source_realpath})
  endforeach(source)

  set(UNITTEST_TEST_SOURCES_REALPATH)

  foreach(source ${unittest-test-sources})
    get_filename_component(source_realpath "${PROJECT_UNITTESTS_DIR}/${source}" REALPATH)
    # string(REGEX REPLACE  "mbed-os/mbed-os" "mbed-os" source_realpath ${source_realpath})
    list(APPEND UNITTEST_TEST_SOURCES_REALPATH ${source_realpath})
  endforeach(source)

  message(STATUS "UNITTEST_INCLUDES_REALPATH")
  prettyPrintList(${UNITTEST_INCLUDES_REALPATH})

  message(STATUS "UNITTEST_SOURCES_REALPATH")
  prettyPrintList(${UNITTEST_SOURCES_REALPATH})

  message(STATUS "UNITTEST_TEST_SOURCES_REALPATH")
  prettyPrintList(${UNITTEST_TEST_SOURCES_REALPATH})

  if (unittest-sources)
    # Create the testable static library.
    add_library("${TEST_SUITE_NAME}.${LIB_NAME}" STATIC ${UNITTEST_SOURCES_REALPATH})
    target_include_directories("${TEST_SUITE_NAME}.${LIB_NAME}" PRIVATE
      ${UNITTEST_INCLUDES_REALPATH})
    set(LIBS_TO_BE_LINKED ${LIBS_TO_BE_LINKED} "${TEST_SUITE_NAME}.${LIB_NAME}")

    # Append lib build directory to list
    list(APPEND BUILD_DIRECTORIES "./CMakeFiles/${TEST_SUITE_NAME}.${LIB_NAME}.dir")
  endif(unittest-sources)

  if (unittest-test-sources)
    # Create the executable.
    add_executable(${TEST_SUITE_NAME} ${UNITTEST_TEST_SOURCES_REALPATH})
    target_include_directories(${TEST_SUITE_NAME} PRIVATE
      ${UNITTEST_INCLUDES_REALPATH})

    # Link the executable with the libraries.
    target_link_libraries(${TEST_SUITE_NAME} ${LIBS_TO_BE_LINKED})

    add_test(NAME "${TEST_SUITE_NAME}" COMMAND ${TEST_SUITE_NAME})

    # Append test build directory to list
    list(APPEND BUILD_DIRECTORIES "./CMakeFiles/${TEST_SUITE_NAME}.dir")
  else()
    message(WARNING "No test source files found for ${TEST_SUITE_NAME}.\n")
  endif(unittest-test-sources)

endforeach(testfile)
