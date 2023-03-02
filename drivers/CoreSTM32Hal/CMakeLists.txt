# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_core(CoreSTM32Hal
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/CoreSTM32Hal.cpp

	PRIVATE_LINK_LIBRARIES
		mbed_headers
)
