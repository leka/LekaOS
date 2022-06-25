# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreQSPI STATIC)

target_include_directories(CoreQSPI
	PUBLIC
	include
)

target_sources(CoreQSPI
	PRIVATE
	source/CoreQSPI.cpp
)

target_link_libraries(CoreQSPI
	mbed-os
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CoreQSPI_test.cpp
	)
endif()
