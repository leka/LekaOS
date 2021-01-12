# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCalculatorKit STATIC)

target_include_directories(LKCalculatorKit
  	PUBLIC
		include
)

target_sources(LKCalculatorKit
  	PRIVATE
    	source/LKCalculatorKit.cpp
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LKCalculatorKit_test.cpp
	)

endif()
