# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCoreTemperatureSensor STATIC)

target_include_directories(LKCoreTemperatureSensor
	PUBLIC
		include
		include/internal
)

target_sources(LKCoreTemperatureSensor
	PRIVATE
		source/LKCoreTemperatureSensor.cpp
		source/LKCoreHTS221Driver.cpp
)

target_link_libraries(LKCoreTemperatureSensor mbed-os)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LKCoreTemperatureSensor_test.cpp
		tests/LKCoreHTS221Driver_test.cpp
	)

endif()
