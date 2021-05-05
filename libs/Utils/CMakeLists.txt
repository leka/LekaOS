# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(Utils STATIC)

target_include_directories(Utils
  	PUBLIC
		include
)

target_sources(Utils
  	PRIVATE
    	source/Utils.cpp
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/Utils_test_map.cpp
		tests/Utils_test_linear_function.cpp
	)

endif()
