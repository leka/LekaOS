# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(RFIDKit STATIC)

target_include_directories(RFIDKit
	PUBLIC
		include
)

target_sources(RFIDKit
	PRIVATE
		source/RFIDKit.cpp
)

target_link_libraries(RFIDKit
	mbed-os
	CoreRFIDReader
)

