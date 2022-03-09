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
		source/Singing.cpp
		source/Sleeping.cpp
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
