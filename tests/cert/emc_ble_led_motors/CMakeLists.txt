# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(certs_emc_ble_led_motors)

target_include_directories(certs_emc_ble_led_motors
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(certs_emc_ble_led_motors
  PRIVATE
    main.cpp
)

target_link_libraries(certs_emc_ble_led_motors driver_Motor)
target_link_libraries(certs_emc_ble_led_motors driver_FastLED)

target_link_libraries(certs_emc_ble_led_motors lib_BLE)

target_link_custom_board_libraries(certs_emc_ble_led_motors)
