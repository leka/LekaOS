# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCoreMicrophone STATIC)

target_include_directories(LKCoreMicrophone
	PUBLIC
		include
)

target_sources(LKCoreMicrophone
	PRIVATE
		source/LKCoreMicrophone.cpp
)

target_link_libraries(LKCoreMicrophone
	PUBLIC
		mbed-os-static
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LKCoreMicrophone_test.cpp
	)

endif()
