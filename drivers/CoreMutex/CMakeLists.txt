# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreMutex STATIC)

target_include_directories(CoreMutex
	PUBLIC
		include
)

target_sources(CoreMutex
	PRIVATE
		source/CoreMutex.cpp
)

target_link_libraries(CoreMutex
	mbed-os
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreMutex_test.cpp
	)

endif()
