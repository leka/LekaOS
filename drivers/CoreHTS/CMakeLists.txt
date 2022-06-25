# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreHTS STATIC)

target_include_directories(CoreHTS
	PUBLIC
	include
	external/include
)

target_sources(CoreHTS
	PRIVATE
	source/CoreHTS.cpp
	external/source/hts221_reg.c
)

target_link_libraries(CoreHTS
	mbed-os
	CoreI2C
	Utils
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CoreHTS_test.cpp
	)
endif()
