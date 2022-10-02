# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreBufferedSerial STATIC)

target_include_directories(CoreBufferedSerial
	PUBLIC
	include
)

target_sources(CoreBufferedSerial
	PRIVATE
	source/CoreBufferedSerial.cpp
)

target_link_libraries(CoreBufferedSerial
	mbed-os
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CoreBufferedSerial_test.cpp
	)
endif()
