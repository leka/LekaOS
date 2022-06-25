# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreEventFlags STATIC)

target_include_directories(CoreEventFlags
	PUBLIC
	include
)

target_sources(CoreEventFlags
	PRIVATE
	source/CoreEventFlags.cpp
)

target_link_libraries(CoreEventFlags mbed-os)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CoreEventFlags_test.cpp
	)
endif()
