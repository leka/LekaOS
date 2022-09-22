# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(ActivityKit STATIC)

target_include_directories(ActivityKit
	PUBLIC
	include
	include/activities
	include/activities/include
)

target_sources(ActivityKit
	PRIVATE
	source/ActivityKit.cpp
	source/activities/DisplayTags.cpp
	source/activities/NumberRecognition.cpp
	source/activities/PictoColorRecognition.cpp
	source/activities/LedColorRecognition.cpp
)

target_link_libraries(ActivityKit
	RFIDKit
	CoreLED
	VideoKit
	ReinforcerKit
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/ActivityKit_test.cpp
	)
endif()
