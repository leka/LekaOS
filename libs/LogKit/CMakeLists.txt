# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LogKit STATIC)

target_include_directories(LogKit
	PUBLIC
		include
)

target_sources(LogKit
	PRIVATE
		source/LogKit.cpp
)

target_link_libraries(LogKit
	mbed-os
	ContainerKit
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LogKit_test_log.cpp
		tests/LogKit_test_fifo.cpp
		tests/LogKit_test_format.cpp
		tests/LogKit_test_now.cpp
		tests/LogKit_test_sink.cpp
	)

endif()
