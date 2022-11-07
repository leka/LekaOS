# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreLL STATIC)

target_include_directories(CoreLL
	PUBLIC
	include
)

target_sources(CoreLL
	PRIVATE
	source/CoreLL.cpp
)

target_link_libraries(CoreLL mbed-os)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CoreLL_test.cpp
	)
endif()
