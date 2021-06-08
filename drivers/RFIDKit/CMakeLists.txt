# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(RFIDKit STATIC)

target_include_directories(RFIDKit
	PUBLIC
		include
)

target_sources(RFIDKit
	PRIVATE
		source/RFIDKit.cpp
)

target_link_libraries(RFIDKit mbed-os)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/RFIDKit_test.cpp
	)

endif()
