# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(target_LEKA_V1_1_DEV STATIC)

target_sources(target_LEKA_V1_1_DEV
  PRIVATE
	PeripheralPins.c
	system_clock.c
)

target_include_directories(target_LEKA_V1_1_DEV
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(target_LEKA_V1_1_DEV mbed-os)
