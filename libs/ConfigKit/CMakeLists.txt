# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(ConfigKit STATIC)

target_include_directories(ConfigKit
	PUBLIC
	include
)

target_sources(ConfigKit
	PRIVATE
	source/Config.cpp
)

target_link_libraries(ConfigKit
	mbed-os
	FileManagerKit
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/Config_test.cpp
	)
endif()
