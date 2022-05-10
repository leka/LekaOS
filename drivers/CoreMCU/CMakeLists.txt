# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreMCU STATIC)

target_include_directories(CoreMCU
	PUBLIC
		include
)

target_sources(CoreMCU
	PRIVATE
		source/CoreMCU.cpp
)

target_link_libraries(CoreMCU mbed-os)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreMCU_test.cpp
	)

endif()
