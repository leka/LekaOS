# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(FileManagerKit STATIC)

target_include_directories(FileManagerKit
	PUBLIC
	include
)

target_sources(FileManagerKit
	PRIVATE
	source/File.cpp
	source/FileReception.cpp
	source/FileManagerKit.cpp
)

target_link_libraries(FileManagerKit
	mbed-os
	ContainerKit
	CoreEventQueue
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/File_test.cpp
		tests/File_initialization_test.cpp
		tests/File_open_close_test.cpp
		tests/File_sha256_test.cpp
		tests/FileReception_test.cpp
		tests/FileManagerKit_test.cpp
	)
endif()
