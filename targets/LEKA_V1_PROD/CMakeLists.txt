# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

set(CUSTOM_TARGET_NAME mbed-leka-v1-prod)

add_library(${CUSTOM_TARGET_NAME} INTERFACE)

target_include_directories(${CUSTOM_TARGET_NAME}
	INTERFACE
		.
)

target_sources(${CUSTOM_TARGET_NAME}
	INTERFACE
		PeripheralPins.c
		system_clock.c
)

target_link_libraries(${CUSTOM_TARGET_NAME}
	INTERFACE
		mbed-stm32f769xi
)
