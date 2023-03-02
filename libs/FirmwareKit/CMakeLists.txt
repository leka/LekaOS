# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${ROOT_DIR}/config/os_version)

file(STRINGS "${ROOT_DIR}/config/os_version" OS_VERSION)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/os_version.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/include/os_version.h")

add_lib_kit(FirmwareKit
	PUBLIC_INCLUDE_DIRS
		include
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>

	PRIVATE_SOURCES
		source/FirmwareKit.cpp

	PRIVATE_LINK_LIBRARIES
		mbed_headers
		CoreFlashMemory
		FileManagerKit

	UNIT_TESTS_SOURCES
		tests/FirmwareKit_test.cpp
)
