# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreInterruptIn STATIC)

target_include_directories(CoreInterruptIn
	PUBLIC
	include
)

target_sources(CoreInterruptIn
	PRIVATE
	source/CoreInterruptIn.cpp
)

target_link_libraries(CoreInterruptIn
	mbed-os
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CoreInterruptIn_test.cpp
	)
endif()
