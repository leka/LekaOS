# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CorePwm STATIC)

target_include_directories(CorePwm
	PUBLIC
	include
)

target_sources(CorePwm
	PRIVATE
	source/CorePwm.cpp
)

target_link_libraries(CorePwm
	mbed-os
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CorePwm_test.cpp
	)
endif()
