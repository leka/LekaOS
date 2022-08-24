# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(MagicCardKit STATIC)

target_include_directories(MagicCardKit
	PUBLIC
	include
	include/activity
)

target_sources(MagicCardKit
	PRIVATE
	source/MagicCardKit.cpp
	source/StateMachineActivity.cpp
	source/activity/SelectReinforcers.cpp
	source/activity/ColorRecognition.cpp
	source/activity/EmotionRecognition.cpp
	source/activity/NumberRecognition.cpp
	source/activity/ShapeRecognition.cpp
	source/activity/DisplayEmotions.cpp
	source/activity/ColorLeka.cpp
	source/activity/TwoNumbersAddition.cpp
	source/activity/TwoNumbersSubstraction.cpp
	source/activity/ColoredShapeRecognition.cpp
)

target_link_libraries(MagicCardKit
	mbed-os
	CoreRFIDReader
	RFIDKit
	LedKit
	BehaviorKit
	ReinforcerKit
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/MagicCardKit_test.cpp
	)
endif()
