# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_file_manager)

target_sources(spike_lk_file_manager
  PRIVATE
    main.cpp
)

target_include_directories(spike_lk_file_manager
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(spike_lk_file_manager lib_FileManager)
target_link_libraries(spike_lk_file_manager lib_HelloWorld)

target_link_custom_leka_targets(spike_lk_file_manager)
