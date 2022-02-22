# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LedKit STATIC)

target_include_directories(LedKit
	PUBLIC
		include
		include/internal
)

target_sources(LedKit
	PRIVATE
		source/LedKit.cpp
)

target_link_libraries(LedKit
	CoreLED
	ColorKit
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
	)

endif()
