# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreMotor STATIC)

target_include_directories(CoreMotor
	PUBLIC
		include
)

target_sources(CoreMotor
	PRIVATE
		source/CoreMotor.cpp
)

target_link_libraries(CoreMotor
	mbed-os
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreMotor_test.cpp
	)

endif()
