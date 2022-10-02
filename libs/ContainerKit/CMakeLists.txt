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
	source/CircularQueue.cpp
)

target_link_libraries(ContainerKit
	PRIVATE
	mbed-os
	CriticalSection
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CircularQueue_test.cpp
		tests/CircularQueue_test_clear.cpp
		tests/CircularQueue_test_drop.cpp
		tests/CircularQueue_test_empty.cpp
		tests/CircularQueue_test_hasPattern.cpp
		tests/CircularQueue_test_peek.cpp
		tests/CircularQueue_test_pop.cpp
		tests/CircularQueue_test_push.cpp
		tests/CircularQueue_test_pushpop.cpp
		tests/CircularQueue_test_size.cpp
	)
endif()
