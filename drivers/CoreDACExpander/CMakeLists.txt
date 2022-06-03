# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreDACExpander STATIC)

target_include_directories(CoreDACExpander
	PUBLIC
		include
)

target_sources(CoreDACExpander
	PRIVATE
		source/CoreDACExpander.cpp
)

target_link_libraries(CoreDACExpander
	mbed-os
	CoreI2C
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreDACExpander_test.cpp
	)

endif()
