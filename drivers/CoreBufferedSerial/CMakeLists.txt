# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_core(CoreBufferedSerial
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/CoreBufferedSerial.cpp

	PRIVATE_LINK_LIBRARIES
		mbed_headers

	UNIT_TESTS_SOURCES
		tests/CoreBufferedSerial_test.cpp
)
