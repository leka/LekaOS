# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_ble)

target_sources(spike_lk_ble
  PRIVATE
    main.cpp
)

target_include_directories(spike_lk_ble
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(spike_lk_ble lib_HelloWorld)
target_link_libraries(spike_lk_ble lib_BLE)

target_link_custom_board_libraries(spike_lk_ble)
