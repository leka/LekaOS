# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreTouch STATIC)

target_include_directories(CoreTouch
	PUBLIC
		include
)

target_sources(CoreTouch
	PRIVATE
		source/CoreTouchReaderMCP23017.cpp
)

target_link_libraries(CoreTouch
		mbed-os
		CoreI2C
)
