# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(EventLoopKit STATIC)

target_include_directories(EventLoopKit
	PUBLIC
	include
)

target_sources(EventLoopKit
	PRIVATE
	source/EventLoopKit.cpp
)

target_link_libraries(EventLoopKit
	mbed-os
	CoreEventFlags
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/EventLoopKit_test.cpp
		tests/EventLoopKit_test_stub.cpp
	)
endif()
