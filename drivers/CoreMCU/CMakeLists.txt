# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_core(CoreMCU
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/CoreMCU.cpp

	PRIVATE_LINK_LIBRARIES
		mbed_headers

	UNIT_TESTS_SOURCES
		tests/CoreMCU_test.cpp
)
