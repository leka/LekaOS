# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreIOExpander STATIC)

target_include_directories(CoreIOExpander
	PUBLIC
	include
)

target_sources(CoreIOExpander
	PRIVATE
	source/CoreIOExpander.cpp
)

target_link_libraries(CoreIOExpander
	mbed-os
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CoreIOExpander_test.cpp
	)
endif()
