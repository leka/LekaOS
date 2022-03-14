# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(FileSystemKit STATIC)

target_include_directories(FileSystemKit
	PUBLIC
		include
)

target_sources(FileSystemKit
	PRIVATE
		source/File.cpp
		source/Config.cpp
)

target_link_libraries(FileSystemKit
	mbed-os
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/File_test.cpp
	)

endif()
