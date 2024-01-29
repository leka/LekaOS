# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(IOKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_INCLUDE_DIRS
		internal

	PRIVATE_SOURCES
		source/DigitalIn.cpp
		source/DigitalOut.cpp
		source/IO.cpp

	PRIVATE_LINK_LIBRARIES
		mbed_headers

	UNIT_TESTS_SOURCES
		tests/DigitalIn_test.cpp
		tests/DigitalOut_test.cpp
)
