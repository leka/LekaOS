# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCoreTemperatureSensor STATIC)

target_include_directories(LKCoreTemperatureSensor
	PUBLIC
		include
		external/include
)

target_sources(LKCoreTemperatureSensor
	PRIVATE
		source/LKCoreTemperatureSensor.cpp
		external/source/hts221_reg.c
)

target_link_libraries(LKCoreTemperatureSensor
		mbed-os
		LKCoreI2C
		LKUtils
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LKCoreTemperatureSensor_test.cpp
	)

endif()
