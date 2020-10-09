# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_bluetooth)

target_sources(spike_lk_bluetooth
  PRIVATE
    main.cpp
)

target_include_directories(spike_lk_bluetooth
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(spike_lk_bluetooth lib_HelloWorld)
target_link_libraries(spike_lk_bluetooth lib_LekaBluetooth)

target_link_target_board_libraries(spike_lk_bluetooth DISCO_ORIGINAL LEKA_V1_0_DEV)
