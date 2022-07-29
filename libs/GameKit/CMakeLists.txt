# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(GameKit STATIC)

target_include_directories(GameKit
	PUBLIC
	include
	include/games
)

target_sources(GameKit
	PRIVATE
	source/GameKit.cpp
)

target_link_libraries(GameKit
	LedKit
	TouchSensorKit
	EventLoopKit
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/GameKit_test.cpp
	)
endif()
