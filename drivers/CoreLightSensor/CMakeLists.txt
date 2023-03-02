# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_core(CoreLightSensor
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/CoreLightSensor.cpp

	PRIVATE_LINK_LIBRARIES
		mbed_headers

	UNIT_TESTS_SOURCES
		tests/CoreLightSensor_test.cpp
)
