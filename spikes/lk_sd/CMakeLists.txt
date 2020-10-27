# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_sd)

target_sources(spike_lk_sd
  PRIVATE
    main.cpp
)

target_include_directories(spike_lk_sd
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(spike_lk_sd driver_SD)
target_link_libraries(spike_lk_sd lib_HelloWorld)

target_link_target_board_libraries(spike_lk_sd DISCO_ORIGINAL LEKA_V1_0_DEV)
