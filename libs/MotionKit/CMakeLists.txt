# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(MotionKit STATIC)

target_include_directories(MotionKit
	PUBLIC
	include
)

target_sources(MotionKit
	PRIVATE
	source/MotionKit.cpp
	source/PID.cpp
)

target_link_libraries(MotionKit
	CoreIMU
	IMUKit
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/MotionKit_test.cpp
		tests/PID_test.cpp
	)
endif()
