# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCoreHTS221Driver STATIC)

target_include_directories(LKCoreHTS221Driver
	PUBLIC
		include
)

target_sources(LKCoreHTS221Driver
	PRIVATE
		source/LKCoreHTS221Driver.cpp
)

target_link_libraries(LKCoreHTS221Driver mbed-os)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LKCoreHTS221Driver_test.cpp
	)

endif()
