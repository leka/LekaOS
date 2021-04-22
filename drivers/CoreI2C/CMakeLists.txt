# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreI2C STATIC)

target_include_directories(CoreI2C
	PUBLIC
		include
)

target_sources(CoreI2C
	PRIVATE
		source/CoreI2C.cpp
)

target_link_libraries(CoreI2C mbed-os)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreI2C_test.cpp
	)

endif()
