# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreTouch STATIC)

target_include_directories(CoreTouch
	PUBLIC
		include
)

target_sources(CoreTouch
	PRIVATE
		source/CoreTouch.cpp
)

target_link_libraries(CoreTouch
		mbed-os
		CoreDACTouch
		CoreIOExpander
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreTouch_test.cpp
	)

endif()
