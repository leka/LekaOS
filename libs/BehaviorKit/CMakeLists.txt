# Leka - LekaOS
# Copyright 2023 APF France handicap
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
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/BehaviorKit_test.cpp
	)
endif()
