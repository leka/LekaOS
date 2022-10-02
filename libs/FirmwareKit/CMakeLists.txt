# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(FirmwareKit STATIC)

target_include_directories(FirmwareKit
	PUBLIC
	include
)

target_sources(FirmwareKit
	PRIVATE
	source/FirmwareKit.cpp
)

target_link_libraries(FirmwareKit
	mbed-os
	FileManagerKit
	CoreFlashMemory
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/FirmwareKit_test.cpp
	)
endif()
