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
		source/CoreGapEventHandler.cpp
		source/CoreGap.cpp
)

target_link_libraries(BLEKit
	mbed-os
	CoreEventQueue
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreGapEventHandler_test.cpp
		tests/CoreGap_test.cpp
		tests/BLEKit_test.cpp
	)

endif()
