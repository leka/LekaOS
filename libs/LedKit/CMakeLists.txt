# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LedKit STATIC)

target_include_directories(LedKit
	PUBLIC
	include
	include/animations
)

target_sources(LedKit
	PRIVATE
	source/animations/AfraidBlue.cpp
	source/animations/AfraidRed.cpp
	source/animations/AfraidRedBlue.cpp
	source/animations/Amazed.cpp
	source/animations/Angry.cpp
	source/animations/AngryShort.cpp
	source/animations/BlinkGreen.cpp
	source/animations/BleConnection.cpp
	source/animations/Bubbles.cpp
	source/animations/Disgusted.cpp
	source/animations/Fire.cpp
	source/animations/Fly.cpp
	source/animations/Happy.cpp
	source/LedKit.cpp
	source/animations/Rainbow.cpp
	source/animations/Sad.cpp
	source/animations/SadCry.cpp
	source/animations/Sick.cpp
	source/animations/Singing.cpp
	source/animations/Sleeping.cpp
	source/animations/Sneeze.cpp
	source/animations/SpinBlink.cpp
	source/animations/Sprinkles.cpp
	source/animations/Underwater.cpp
	source/animations/WakeUp.cpp
	source/animations/Wink.cpp
	source/animations/Yawn.cpp
)

target_link_libraries(LedKit
	CoreLED
	ColorKit
	EventLoopKit
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/LedKit_test.cpp
		tests/LedKit_test_animations.cpp
		tests/LedKit_test_normalizeStep.cpp
	)
endif()
