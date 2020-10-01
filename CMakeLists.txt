# Leka - LekaOS
# Copyright 2020 APF France handicap
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

# Add custom target subdirectory
add_target_board_subdirectory("${TARGET_BOARD}")

# Add libraries
add_subdirectory(${LIB_DIR}/HelloWorld)

add_subdirectory(${LIB_DIR}/BLE)
add_subdirectory(${LIB_DIR}/Led)
add_subdirectory(${LIB_DIR}/Motor)

add_subdirectory(${LIB_DIR}/LekaRFID)
add_subdirectory(${LIB_DIR}/LekaWifi)
add_subdirectory(${LIB_DIR}/LekaTouch)
add_subdirectory(${LIB_DIR}/LekaScreen)
add_subdirectory(${LIB_DIR}/LekaFirmware)
add_subdirectory(${LIB_DIR}/LekaBluetooth)

# Add the main source files of the project from ./src
add_subdirectory(${ROOT_DIR}/src)

# If you use spikes to test features, you can add them here
add_subdirectory(${ROOT_DIR}/spike/blinky)
add_subdirectory(${ROOT_DIR}/spike/watchdog)

add_subdirectory(${ROOT_DIR}/spike/investigation_day_ble)
add_subdirectory(${ROOT_DIR}/spike/investigation_day_lcd)
add_subdirectory(${ROOT_DIR}/spike/investigation_day_led)
add_subdirectory(${ROOT_DIR}/spike/investigation_day_qspi)
add_subdirectory(${ROOT_DIR}/spike/investigation_day_rfid)
add_subdirectory(${ROOT_DIR}/spike/investigation_day_wifi)
add_subdirectory(${ROOT_DIR}/spike/investigation_day_motors)
add_subdirectory(${ROOT_DIR}/spike/investigation_day_bluetooth)
add_subdirectory(${ROOT_DIR}/spike/investigation_day_sensors_touch)

add_subdirectory(${ROOT_DIR}/spike/investigation_day_ble_led_motors)

# Finally print the mbed-cmake build report
mbed_cmake_print_build_report()
