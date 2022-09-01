# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(ActivityKit STATIC)

target_include_directories(ActivityKit
	PUBLIC
	include
	include/activities
)

target_sources(ActivityKit
	PRIVATE
	source/ActivityKit.cpp
)

target_link_libraries(ActivityKit
	RFIDKit
	CoreLED
	VideoKit
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/ActivityKit_test.cpp
	)
endif()
