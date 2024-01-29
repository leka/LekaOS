# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_core(CoreMotor
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/CoreMotor.cpp

	PRIVATE_LINK_LIBRARIES
		mbed_headers

	UNIT_TESTS_SOURCES
		tests/CoreMotor_test.cpp
)
