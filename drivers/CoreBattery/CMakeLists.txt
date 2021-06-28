# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreBattery STATIC)

target_include_directories(CoreBattery
	PUBLIC
		include
)

target_sources(CoreBattery
	PRIVATE
		source/CoreBattery.cpp
)

target_link_libraries(CoreBattery mbed-os)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreBattery_test.cpp
	)

endif()
