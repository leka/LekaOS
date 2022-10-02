# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(BatteryKit STATIC)

target_include_directories(BatteryKit
	PUBLIC
	include
)

target_sources(BatteryKit
	PRIVATE
	source/BatteryKit.cpp
)

target_link_libraries(BatteryKit
	mbed-os
	CoreEventQueue
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/BatteryKit_test.cpp
	)
endif()
