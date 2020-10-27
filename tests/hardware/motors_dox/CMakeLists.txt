# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(hardware_motors_dox)

target_include_directories(hardware_motors_dox
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(hardware_motors_dox
  PRIVATE
    main.cpp
)

target_link_libraries(hardware_motors_dox driver_Motor)

target_link_libraries(hardware_motors_dox lib_LekaMotors)
target_link_libraries(hardware_motors_dox lib_LekaSD)

target_link_target_board_libraries(hardware_motors_dox DISCO_ORIGINAL LEKA_V1_0_DEV_led_motors)
