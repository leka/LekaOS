# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(TouchSensorKit STATIC)

target_include_directories(TouchSensorKit
	PUBLIC
		include
)

target_sources(TouchSensorKit
	PRIVATE
		source/TouchSensorKit.cpp
)

target_link_libraries(TouchSensorKit
	mbed-os
	IOKit
	CoreI2C
	CoreIOExpander
	CoreQDAC
	CoreTouchSensor
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/TouchSensorKit_test.cpp
	)

endif()
