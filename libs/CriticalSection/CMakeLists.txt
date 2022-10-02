# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CriticalSection STATIC)

target_include_directories(CriticalSection
	PUBLIC
		include
)

target_sources(CriticalSection
	PRIVATE
		source/CriticalSection.cpp
)

target_link_libraries(CriticalSection
	mbed-os
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CriticalSection_test.cpp
	)

endif()
