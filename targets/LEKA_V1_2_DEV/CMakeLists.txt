# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-leka-v1-2-dev INTERFACE)

target_include_directories(mbed-leka-v1-2-dev
	INTERFACE
	.
)

target_sources(mbed-leka-v1-2-dev
	INTERFACE
		PeripheralPins.c
		system_clock.c
)

target_link_libraries(mbed-leka-v1-2-dev
	INTERFACE
		mbed-stm32f769xi
)
