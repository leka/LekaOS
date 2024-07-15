# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_core(CoreI2C
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/CoreI2C.cpp

	PRIVATE_LINK_LIBRARIES
		mbed_headers

	UNIT_TESTS_SOURCES
		tests/CoreI2C_test.cpp
)
