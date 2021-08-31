# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(IOKit STATIC)

target_include_directories(IOKit
	PUBLIC
		include
	PRIVATE
		internal
)

target_sources(IOKit
	PRIVATE
		source/IO.cpp
		source/DigitalIn.cpp
)

target_link_libraries(IOKit
	PRIVATE
		mbed-os

)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/DigitalIn_test.cpp
	)

endif()
