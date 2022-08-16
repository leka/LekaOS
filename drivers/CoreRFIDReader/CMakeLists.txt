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
	source/CoreRFIDReaderCR95HF.cpp
)

target_link_libraries(CoreRFIDReader
	mbed-os
	CoreBufferedSerial
	CoreEventQueue
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CoreRFIDReaderCR95HF_test.cpp
	)
endif()
