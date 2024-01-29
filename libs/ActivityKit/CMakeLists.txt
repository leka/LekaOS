# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(ActivityKit
	PUBLIC_INCLUDE_DIRS
		include
		include/activities
		include/activities/include

	PRIVATE_SOURCES
		source/ActivityKit.cpp
		source/activities/DisplayTags.cpp
		source/activities/NumberRecognition.cpp
		source/activities/PictoColorRecognition.cpp
		source/activities/LedColorRecognition.cpp
		source/activities/ChooseReinforcer.cpp
		source/activities/EmotionRecognition.cpp
		source/activities/FoodRecognition.cpp
		source/activities/LedNumberCounting.cpp
		source/activities/FlashNumberCounting.cpp
		source/activities/SuperSimon.cpp
		source/activities/ShapeRecognition.cpp

	PRIVATE_LINK_LIBRARIES
		CoreLED
		LedKit
		RFIDKit
		ReinforcerKit

	UNIT_TESTS_SOURCES
		tests/ActivityKit_test.cpp
)
