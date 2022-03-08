# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LedKit STATIC)

target_include_directories(LedKit
	PUBLIC
		include
		include/internal
)

target_sources(LedKit
	PRIVATE
		source/AfraidBlue.cpp
		source/AfraidRed.cpp
		source/AfraidRedBlue.cpp
		source/Amazed.cpp
		source/Angry.cpp
		source/AngryShort.cpp
		source/Bubbles.cpp
		source/Disgusted.cpp
		source/Fly.cpp
		source/Happy.cpp
		source/LedKit.cpp
		source/LoadingGreen.cpp
		source/LoadingOrange.cpp
		source/LoadingRed.cpp
		source/LoadingYellow.cpp
		source/Sad.cpp
		source/SadCry.cpp
		source/Sick.cpp
		source/Singing.cpp
		source/Sleeping.cpp
		source/Sneeze.cpp
		source/Underwater.cpp
		source/Waiting.cpp
		source/WakeUp.cpp
		source/Wink.cpp
		source/Yawn.cpp
)

target_link_libraries(LedKit
	CoreLED
	ColorKit
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LedKit_test.cpp
		tests/LedKit_test_animations.cpp
	)

endif()
