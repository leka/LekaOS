# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreLightSensor STATIC)

target_include_directories(CoreLightSensor
	PUBLIC
	include
)

target_sources(CoreLightSensor
	PRIVATE
	source/CoreLightSensor.cpp
)

target_link_libraries(CoreLightSensor mbed-os)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CoreLightSensor_test.cpp
	)
endif()
