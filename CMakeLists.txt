# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

# mbed-cmake requires at least CMake 3.18
cmake_minimum_required(VERSION 3.18)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

# ⚠️ Before all, include mbed-cmake.cmake
include(./mbed-cmake.cmake)

# Then configure name of the project
project(LekaOS LANGUAGES C CXX ASM)

# Generate compile commands database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# For convenience you can define useful variables
set(LIBS_DIR    ${ROOT_DIR}/libs)
set(SPIKES_DIR  ${ROOT_DIR}/spikes)
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
add_subdirectory(${LIBS_DIR}/HardwareTest/LekaMotors)
add_subdirectory(${LIBS_DIR}/HardwareTest/LekaSD)

# Add the main source files of the project from ./src
add_subdirectory(${ROOT_DIR}/src)

# If you use spikes to test features, you can add them here
add_subdirectory(${SPIKES_DIR}/mbed_blinky)
add_subdirectory(${SPIKES_DIR}/mbed_watchdog)

add_subdirectory(${SPIKES_DIR}/lk_ble)
add_subdirectory(${SPIKES_DIR}/lk_lcd)
add_subdirectory(${SPIKES_DIR}/lk_led)
add_subdirectory(${SPIKES_DIR}/lk_qspi)
add_subdirectory(${SPIKES_DIR}/lk_rfid)
add_subdirectory(${SPIKES_DIR}/lk_wifi)
add_subdirectory(${SPIKES_DIR}/lk_motors)
add_subdirectory(${SPIKES_DIR}/lk_bluetooth)
add_subdirectory(${SPIKES_DIR}/lk_sensors_touch)

add_custom_target(spikes)
add_dependencies( spikes
	spike_lk_ble
	spike_lk_lcd
	spike_lk_led
	spike_lk_qspi
	spike_lk_rfid
	spike_lk_wifi
	spike_lk_motors
	spike_lk_bluetooth
	spike_lk_sensors_touch
)

add_subdirectory(${ROOT_DIR}/tests/cert/emc_ble_led_motors)
add_subdirectory(${ROOT_DIR}/tests/cert/emc_lcd_led_motors)
add_subdirectory(${ROOT_DIR}/tests/cert/emc_bt_lcd_qspi_rfid_touch_wifi)
add_subdirectory(${ROOT_DIR}/tests/hardware/motors_dox)

# Finally print the mbed-cmake build report
mbed_cmake_print_build_report()
