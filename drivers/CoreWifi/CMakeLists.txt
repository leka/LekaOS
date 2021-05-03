# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreWifi STATIC)

target_include_directories(CoreWifi
	PUBLIC
		include
)

target_sources(CoreWifi
	PRIVATE
		source/CoreWifi.cpp
)

target_link_libraries(CoreWifi
		mbed-os
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreWifi_test.cpp
	)

endif()
