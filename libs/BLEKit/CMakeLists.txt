# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(BLEKit STATIC)

target_include_directories(BLEKit
	PUBLIC
		include
)

target_sources(BLEKit
	PRIVATE
		source/BLEKit.cpp
)

target_link_libraries(BLEKit
	PRIVATE
		mbed-os
)
