# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreIMUlsm6dsox STATIC)

target_include_directories(CoreIMUlsm6dsox
	PUBLIC
	include
	extern/include
)

target_sources(CoreIMUlsm6dsox
	PRIVATE
	extern/source/lsm6dsox_reg.c
	source/CoreIMUlsm6dsox.cpp
)

target_link_libraries(CoreIMUlsm6dsox)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
	)
endif()
