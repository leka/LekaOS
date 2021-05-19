# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CircularBuffer STATIC)

target_include_directories(CircularBuffer
	PUBLIC
		include
)

target_sources(CircularBuffer
	PRIVATE
		source/CircularBuffer.cpp
)

target_link_libraries(CircularBuffer
	PRIVATE
		mbed-os
		CriticalSection
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CircularBuffer_test.cpp
	)

endif()
