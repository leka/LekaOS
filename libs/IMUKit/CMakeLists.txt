# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(external)

add_library(IMUKit STATIC)

target_include_directories(IMUKit
	PUBLIC
	include
)

target_sources(IMUKit
	PRIVATE
	source/IMUKit.cpp
)

target_link_libraries(IMUKit
	CoreIMU
	Fusion
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/IMUKit_test.cpp
	)
endif()
