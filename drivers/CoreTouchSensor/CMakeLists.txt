# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreTouchSensor STATIC)

target_include_directories(CoreTouchSensor
	PUBLIC
	include
)

target_sources(CoreTouchSensor
	PRIVATE
	source/CoreTouchSensor.cpp
)

target_link_libraries(CoreTouchSensor
	PRIVATE
	mbed-os
	CoreI2C
	CoreIOExpander
	CoreQDAC
	IOKit
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CoreTouchSensor_test.cpp
	)
endif()
