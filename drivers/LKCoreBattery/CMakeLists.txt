# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCoreBattery STATIC)

target_include_directories(LKCoreBattery
	PUBLIC
		include
)

target_sources(LKCoreBattery
	PRIVATE
		source/LKCoreBattery.cpp
)

target_link_libraries(LKCoreBattery mbed-os)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LKCoreBattery_test.cpp
	)

endif()
