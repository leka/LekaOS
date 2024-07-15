# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_core(CoreInterruptIn
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/CoreInterruptIn.cpp

	PRIVATE_LINK_LIBRARIES
		mbed_headers

	UNIT_TESTS_SOURCES
		tests/CoreInterruptIn_test.cpp
)
