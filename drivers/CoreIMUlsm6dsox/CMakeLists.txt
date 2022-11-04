# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreIMULSM6DSOX STATIC)

target_include_directories(CoreIMULSM6DSOX
	PUBLIC
	include
	extern/include
)

target_sources(CoreIMULSM6DSOX
	PRIVATE
	extern/source/lsm6dsox_reg.c
	source/CoreIMULSM6DSOX.cpp
)

target_link_libraries(CoreIMULSM6DSOX)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
	)
endif()
