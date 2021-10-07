# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(ContainerKit STATIC)

target_include_directories(ContainerKit
	PUBLIC
		include
)

target_sources(ContainerKit
	PRIVATE
		source/CircularBuffer.cpp
)

target_link_libraries(ContainerKit
	PRIVATE
		mbed-os
		CriticalSection
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CircularBuffer_test.cpp
	)

endif()
