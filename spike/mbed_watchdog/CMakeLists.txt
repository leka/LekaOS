# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_watchdog)

target_sources(spike_watchdog
  PRIVATE
    main.cpp
)

target_include_directories(spike_watchdog
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# target_link_libraries(spike_watchdog lib_HelloWorld)
target_link_libraries(spike_watchdog lib_BLE)

target_link_target_board_libraries(spike_watchdog DISCO_ORIGINAL LEKA_V1_0_DEV)
