# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(ReinforcerKit STATIC)

target_include_directories(ReinforcerKit
	PUBLIC
	include
)

target_sources(ReinforcerKit
	PRIVATE
	source/ReinforcerKit.cpp
)

target_link_libraries(ReinforcerKit
	BehaviorKit
	RFIDKit
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/ReinforcerKit_test.cpp
	)
endif()
