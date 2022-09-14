# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(FirmwareKit STATIC)

set(OS_VERSION ${FIRMWARE_VERSION})
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/OSVersion.h.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/OSVersion.h")

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
