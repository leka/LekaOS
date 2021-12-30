# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCoreLL STATIC)

target_include_directories(LKCoreLL
	PUBLIC
		include
)

target_sources(LKCoreLL
	PRIVATE
    	source/LKCoreLL.cpp
)

target_link_libraries(LKCoreLL
	PUBLIC
		mbed-os-static
)


if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LKCoreLL_test.cpp
	)

endif()
