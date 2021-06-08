# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(KitRFID STATIC)

target_include_directories(KitRFID
	PUBLIC
		include
)

target_sources(KitRFID
	PRIVATE
		source/KitRFID.cpp
)

target_link_libraries(KitRFID mbed-os)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/KitRFID_test.cpp
	)

endif()
