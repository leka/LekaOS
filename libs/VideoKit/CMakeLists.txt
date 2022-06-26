# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(VideoKit STATIC)

target_include_directories(VideoKit
	PUBLIC
	include
)

target_sources(VideoKit
	PRIVATE
	source/VideoKit.cpp
)

target_link_libraries(VideoKit
	CoreLL
	CoreSTM32Hal
	CoreVideo
	FileManagerKit
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/VideoKit_test.cpp
	)
endif()
