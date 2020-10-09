# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(certs_emc_lcd_led_motors)

target_include_directories(certs_emc_lcd_led_motors
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(certs_emc_lcd_led_motors
  PRIVATE
    main.cpp
)

target_link_libraries(certs_emc_lcd_led_motors driver_Motor)
target_link_libraries(certs_emc_lcd_led_motors driver_FastLED)

target_link_libraries(certs_emc_lcd_led_motors lib_LekaScreen)

target_link_target_board_libraries(certs_emc_lcd_led_motors DISCO_ORIGINAL LEKA_V1_0_DEV_led_motors)
