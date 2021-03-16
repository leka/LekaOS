# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCoreLightSensor STATIC)

target_include_directories(LKCoreLightSensor
	PUBLIC
		include
)

target_sources(LKCoreLightSensor
	PRIVATE
		source/LKCoreLightSensor.cpp
)

target_link_libraries(LKCoreLightSensor mbed-os)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LKCoreLightSensor_test.cpp
	)

endif()
