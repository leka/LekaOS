# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(FileReception STATIC)

target_include_directories(FileReception
	PUBLIC
		include
)

target_sources(FileReception
	PRIVATE
		source/FileReception.cpp
)

target_link_libraries(FileReception
	PRIVATE
		mbed-os
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/FileReception_test.cpp
	)

endif()
