# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_mbed_watchdog_ticker_vs_thread)

target_include_directories(spike_mbed_watchdog_ticker_vs_thread
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(spike_mbed_watchdog_ticker_vs_thread
  PRIVATE
    main.cpp
)

target_link_target_board_libraries(spike_mbed_watchdog_ticker_vs_thread DISCO_ORIGINAL LEKA_V1_0_DEV)
