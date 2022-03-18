# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(BehaviorKit STATIC)

target_include_directories(BehaviorKit
	PUBLIC
		include
)

target_sources(BehaviorKit
	PRIVATE
		source/BehaviorKit.cpp
)

target_link_libraries(BehaviorKit
	LedKit
	VideoKit
	CoreMotor
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/BehaviorKit_test.cpp
	)

endif()
