# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreMicrophone STATIC)

target_include_directories(CoreMicrophone
	PUBLIC
	include
)

target_sources(CoreMicrophone
	PRIVATE
	source/CoreMicrophone.cpp
)

target_link_libraries(CoreMicrophone mbed-os)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CoreMicrophone_test.cpp
	)
endif()
