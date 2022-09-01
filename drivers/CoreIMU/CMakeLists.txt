# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreIMU STATIC)

target_include_directories(CoreIMU
	PUBLIC
	include
	extern/include
)

target_sources(CoreIMU
	PRIVATE
	source/CoreIMU.cpp
	extern/source/lsm6dsox_reg.c
)

target_link_libraries(CoreIMU)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
	)
endif()
