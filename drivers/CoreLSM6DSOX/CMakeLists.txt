# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreLSM6DSOX STATIC)

target_include_directories(CoreLSM6DSOX
	PUBLIC
	include
	extern/include
)

target_sources(CoreLSM6DSOX
	PRIVATE
	source/CoreLSM6DSOX.cpp
	source/CoreAccelerometer.cpp
	source/CoreGyroscope.cpp
	extern/source/lsm6dsox_reg.c
)

target_link_libraries(CoreLSM6DSOX)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
	)
endif()
