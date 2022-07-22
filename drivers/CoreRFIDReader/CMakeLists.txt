# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreRFIDReader STATIC)

target_include_directories(CoreRFIDReader
	PUBLIC
	include
)

target_sources(CoreRFIDReader
	PRIVATE
	source/CoreRFIDReader.cpp
)

target_link_libraries(CoreRFIDReader
	mbed-os
	CoreBufferedSerial
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CoreRFIDReader_test.cpp
	)
endif()
