# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(FirmwareKit STATIC)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${ROOT_DIR}/config/os_version)

file(STRINGS "${ROOT_DIR}/config/os_version" OS_VERSION)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/os_version.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/include/os_version.h")

target_include_directories(FirmwareKit
	PUBLIC
	include
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
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
