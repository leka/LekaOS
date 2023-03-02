# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(ContainerKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/CircularQueue.cpp

	PRIVATE_LINK_LIBRARIES
		mbed_headers
		CriticalSection

	UNIT_TESTS_SOURCES
		tests/CircularQueue_test_clear.cpp
		tests/CircularQueue_test_drop.cpp
		tests/CircularQueue_test_empty.cpp
		tests/CircularQueue_test_hasPattern.cpp
		tests/CircularQueue_test_peek.cpp
		tests/CircularQueue_test_pop.cpp
		tests/CircularQueue_test_push.cpp
		tests/CircularQueue_test_pushpop.cpp
		tests/CircularQueue_test_size.cpp
		tests/CircularQueue_test.cpp
)
