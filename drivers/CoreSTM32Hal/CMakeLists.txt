# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreSTM32Hal STATIC)

target_include_directories(CoreSTM32Hal
	PUBLIC
	include
)

target_sources(CoreSTM32Hal
	PRIVATE
	source/CoreSTM32Hal.cpp
)

target_link_libraries(CoreSTM32Hal mbed-os)
