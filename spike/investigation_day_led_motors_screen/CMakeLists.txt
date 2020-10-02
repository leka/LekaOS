# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_investigation_day_led_motors_screen)

target_include_directories(spike_investigation_day_led_motors_screen
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(spike_investigation_day_led_motors_screen
  PRIVATE
    main.cpp
)

target_link_libraries(spike_investigation_day_led_motors_screen lib_Led)
target_link_libraries(spike_investigation_day_led_motors_screen lib_Motor)
target_link_libraries(spike_investigation_day_led_motors_screen lib_LekaScreen)

target_link_target_board_libraries(spike_investigation_day_led_motors_screen DISCO_ORIGINAL LEKA_V1_0_DEV_led_motors)
