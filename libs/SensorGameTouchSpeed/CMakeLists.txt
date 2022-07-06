# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(SensorGameTouchSpeed STATIC)

target_include_directories(SensorGameTouchSpeed
	PUBLIC
	include
)

target_sources(SensorGameTouchSpeed
	PRIVATE
	source/TouchSpeed.cpp
)

target_link_libraries(SensorGameTouchSpeed

	TouchSensorKit
	ColorKit
	CoreLED
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/TouchSpeed_test.cpp
	)
endif()
