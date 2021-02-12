# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(driver_LKCoreVideo STATIC)

target_include_directories(driver_LKCoreVideo
  PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(driver_LKCoreVideo
  PRIVATE
    source/LKCoreVideo.cpp
)

target_link_libraries(driver_LKCoreVideo mbed-os)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/LKCoreDMA2D_test.cpp
	)

endif()