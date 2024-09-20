# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(BehaviorKitDeprecated STATIC)

target_include_directories(BehaviorKitDeprecated
	PUBLIC
	include
)

target_sources(BehaviorKitDeprecated
	PRIVATE
	source/BehaviorKitDeprecated.cpp
)

target_link_libraries(BehaviorKitDeprecated
	LedKit
	VideoKit
	CoreMotor
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/BehaviorKitDeprecated_test.cpp
	)
endif()
