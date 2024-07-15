# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_core(CoreHTS
	PUBLIC_INCLUDE_DIRS
		include
		external/include

	PRIVATE_SOURCES
		source/CoreHTS.cpp
		external/source/hts221_reg.c

	PRIVATE_LINK_LIBRARIES
		mbed_headers
		CoreI2C
		Utils

	UNIT_TESTS_SOURCES
		tests/CoreHTS_test.cpp
)
