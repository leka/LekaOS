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
		source/Bubbles.cpp
		source/Fly.cpp
		source/LedKit.cpp
		source/Singing.cpp
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
