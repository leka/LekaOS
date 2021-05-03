# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCoreRFID STATIC)

target_include_directories(LKCoreRFID
	PUBLIC
		include
)

target_sources(LKCoreRFID
	PRIVATE
		source/LKCoreRFID.cpp
)

target_link_libraries(LKCoreRFID mbed-os)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LKCoreRFID_test.cpp
	)

endif()
