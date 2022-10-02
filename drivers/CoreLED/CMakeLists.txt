# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreLED STATIC)

target_include_directories(CoreLED
	PUBLIC
	include
)

target_sources(CoreLED
	PRIVATE
	source/CoreLED.cpp
)

target_link_libraries(CoreLED
	CoreSPI
	ColorKit
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CoreLED_test_brightness.cpp
		tests/CoreLED_test_setColor.cpp
		tests/CoreLED_test_showHideColor.cpp
		tests/CoreLED_test_status.cpp
		tests/CoreLED_test.cpp
	)
endif()
