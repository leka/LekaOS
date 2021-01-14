# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(hardware_flex_test_bluetooth_serial)

target_include_directories(hardware_flex_test_bluetooth_serial
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(hardware_flex_test_bluetooth_serial
  PRIVATE
    main.cpp
)

target_link_libraries(hardware_flex_test_bluetooth_serial driver_FastLED)

target_link_libraries(hardware_flex_test_bluetooth_serial lib_LekaBluetooth)
target_link_libraries(hardware_flex_test_bluetooth_serial lib_LekaRFID)
target_link_libraries(hardware_flex_test_bluetooth_serial lib_LekaTouch)

target_link_custom_board_libraries(hardware_flex_test_bluetooth_serial)