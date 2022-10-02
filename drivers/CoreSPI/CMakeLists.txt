# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreSPI STATIC)

target_include_directories(CoreSPI
	PUBLIC
		include
)

target_sources(CoreSPI
	PRIVATE
		source/CoreSPI.cpp
)

target_link_libraries(CoreSPI
		mbed-os
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreSPI_test.cpp
	)

endif()
