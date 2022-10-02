# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(target_LEKA_V1_2_DEV STATIC)

target_include_directories(target_LEKA_V1_2_DEV
	PUBLIC
    	.
)

target_sources(target_LEKA_V1_2_DEV
	PRIVATE
		PeripheralPins.c
		system_clock.c
)

target_link_libraries(target_LEKA_V1_2_DEV mbed-os)
