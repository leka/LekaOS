# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_rfid)

target_sources(spike_lk_rfid
  PRIVATE
    main.cpp
)

target_include_directories(spike_lk_rfid
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(spike_lk_rfid lib_HelloWorld)
target_link_libraries(spike_lk_rfid lib_LekaRFID)

target_link_target_board_libraries(spike_lk_rfid DISCO_ORIGINAL LEKA_V1_0_DEV)
