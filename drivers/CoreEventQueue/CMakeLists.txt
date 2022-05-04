# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreEventQueue STATIC)

target_include_directories(CoreEventQueue
	PUBLIC
		include
)

target_sources(CoreEventQueue
	PRIVATE
		source/CoreEventQueue.cpp
)

target_link_libraries(CoreEventQueue mbed-os)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreEventQueue_test.cpp
	)

endif()
