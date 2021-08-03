# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreDACTouch STATIC)

target_include_directories(CoreDACTouch
	PUBLIC
		include
)

target_sources(CoreDACTouch
	PRIVATE
		source/CoreDACTouch.cpp
)

target_link_libraries(CoreDACTouch
		mbed-os
		CoreI2C
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreDACTouch_test.cpp
	)

endif()
