# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(TouchSensorKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/TouchSensorKit.cpp

	PUBLIC_LINK_LIBRARIES
		CoreEventQueue

	PRIVATE_LINK_LIBRARIES
		mbed_headers
		CoreTouchSensor

	UNIT_TESTS_SOURCES
		tests/TouchSensorKit_test.cpp
)
