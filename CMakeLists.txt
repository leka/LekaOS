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
set(LIBS_DIR ${ROOT_DIR}/libs)
set(DRIVERS_DIR ${ROOT_DIR}/drivers)
set(TARGETS_DIR ${ROOT_DIR}/targets)

# Add custom target subdirectory
add_target_board_subdirectory("${TARGET_BOARD}")

# Add drivers
add_subdirectory(${DRIVERS_DIR}/FastLED)
add_subdirectory(${DRIVERS_DIR}/Motor)

# Add libraries
add_subdirectory(${LIBS_DIR}/HelloWorld)

add_subdirectory(${LIBS_DIR}/InvestigationDay/BLE)

add_subdirectory(${LIBS_DIR}/InvestigationDay/LekaRFID)
add_subdirectory(${LIBS_DIR}/InvestigationDay/LekaWifi)
add_subdirectory(${LIBS_DIR}/InvestigationDay/LekaTouch)
add_subdirectory(${LIBS_DIR}/InvestigationDay/LekaScreen)
add_subdirectory(${LIBS_DIR}/InvestigationDay/LekaFirmware)
add_subdirectory(${LIBS_DIR}/InvestigationDay/LekaBluetooth)

# Add the main source files of the project from ./src
add_subdirectory(${ROOT_DIR}/src)

# If you use spikes to test features, you can add them here
add_subdirectory(${ROOT_DIR}/spike/mbed_blinky)
add_subdirectory(${ROOT_DIR}/spike/mbed_watchdog)

add_subdirectory(${ROOT_DIR}/spike/lk_ble)
add_subdirectory(${ROOT_DIR}/spike/lk_lcd)
add_subdirectory(${ROOT_DIR}/spike/lk_led)
add_subdirectory(${ROOT_DIR}/spike/lk_qspi)
add_subdirectory(${ROOT_DIR}/spike/lk_rfid)
add_subdirectory(${ROOT_DIR}/spike/lk_wifi)
add_subdirectory(${ROOT_DIR}/spike/lk_motors)
add_subdirectory(${ROOT_DIR}/spike/lk_bluetooth)
add_subdirectory(${ROOT_DIR}/spike/lk_sensors_touch)

add_subdirectory(${ROOT_DIR}/tests/cert/emc_ble_led_motors)
add_subdirectory(${ROOT_DIR}/tests/cert/emc_lcd_led_motors)
add_subdirectory(${ROOT_DIR}/tests/cert/emc_bt_lcd_qspi_rfid_touch_wifi)

# Finally print the mbed-cmake build report
mbed_cmake_print_build_report()
