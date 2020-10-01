# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_investigation_day_ble_led_motors)

target_include_directories(spike_investigation_day_ble_led_motors
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(spike_investigation_day_ble_led_motors
  PRIVATE
    main.cpp
)

target_link_libraries(spike_investigation_day_ble_led_motors lib_Led)
target_link_libraries(spike_investigation_day_ble_led_motors lib_BLE)
target_link_libraries(spike_investigation_day_ble_led_motors lib_Motor)

target_link_target_board_libraries(spike_investigation_day_ble_led_motors DISCO_ORIGINAL LEKA_V1_0_DEV_led_motors)
