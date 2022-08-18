# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreQDAC STATIC)

target_include_directories(CoreQDAC
	PUBLIC
	include
)

target_sources(CoreQDAC
	PRIVATE
	source/CoreQDAC.cpp
)

target_link_libraries(CoreQDAC
	mbed-os
	CoreI2C
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CoreQDAC_test.cpp
	)
endif()
