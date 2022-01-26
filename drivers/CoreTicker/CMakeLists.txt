# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreTicker STATIC)

target_include_directories(CoreTicker
	PUBLIC
		include
)

target_sources(CoreTicker
	PRIVATE
		source/CoreTicker.cpp
)

target_link_libraries(CoreTicker
	mbed-os
)
