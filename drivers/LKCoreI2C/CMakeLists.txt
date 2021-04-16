# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCoreI2C STATIC)

target_include_directories(LKCoreI2C
	PUBLIC
		include
)

target_sources(LKCoreI2C
	PRIVATE
		source/LKCoreI2C.cpp
)

target_link_libraries(LKCoreI2C mbed-os)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LKCoreI2C_test.cpp
	)

endif()
