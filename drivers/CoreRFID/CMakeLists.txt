# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreRFID STATIC)

target_include_directories(CoreRFID
	PUBLIC
		include
)

target_sources(CoreRFID
	PRIVATE
		source/CoreRFID.cpp
)

target_link_libraries(CoreRFID
	CoreBufferedSerial
	mbed-os
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreRFID_test.cpp
	)

endif()
