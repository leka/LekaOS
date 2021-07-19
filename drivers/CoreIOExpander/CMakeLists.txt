# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreIOExpander STATIC)

target_include_directories(CoreIOExpander
	PUBLIC
		include
)

target_sources(CoreIOExpander
	PRIVATE
		source/MCP23017.cpp
)

target_link_libraries(CoreIOExpander
		mbed-os
		CoreI2C
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/MCP23017_test.cpp
	)

endif()
