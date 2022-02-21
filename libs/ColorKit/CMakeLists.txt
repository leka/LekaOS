# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(ColorKit STATIC)
add_subdirectory(dependencies)

target_include_directories(ColorKit
	PUBLIC
		include
)

target_sources(ColorKit
	PRIVATE
		source/ColorKit.cpp
)

target_link_libraries(ColorKit
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/RGB_test.cpp
	)

endif()
