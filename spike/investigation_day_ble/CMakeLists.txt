# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_investigation_day_ble)

target_sources(spike_investigation_day_ble
  PRIVATE
    main.cpp
)

target_include_directories(spike_investigation_day_ble
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(spike_investigation_day_ble lib_HelloWorld)
target_link_libraries(spike_investigation_day_ble lib_BLE)

target_link_target_board_libraries(spike_investigation_day_ble DISCO_ORIGINAL LEKA_V1_0_DEV)
