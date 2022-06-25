# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(UIAnimationKit STATIC)

target_include_directories(UIAnimationKit
	PUBLIC
	include
	include/internal
)

target_sources(UIAnimationKit
	PRIVATE
	source/BouncingSquare.cpp
	source/UIAnimationKit.cpp
)

target_link_libraries(UIAnimationKit
	CoreLL
	CoreSTM32Hal
	CoreVideo
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/UIAnimationKit_test.cpp
	)
endif()
