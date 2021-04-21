# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKUtils STATIC)

target_include_directories(LKUtils
  	PUBLIC
		include
)

target_sources(LKUtils
  	PRIVATE
    	source/LKUtils.cpp
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LKUtils_test_map.cpp
		tests/LKUtils_test_linear_function.cpp
	)

endif()
