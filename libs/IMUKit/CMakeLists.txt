# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(IMUKit STATIC)

target_include_directories(IMUKit
	PUBLIC
	include
)

target_sources(IMUKit
	PRIVATE
	source/class.cpp
	source/decision_trees.cpp
	source/IMUKit.cpp
	source/ODRCard.cpp
)

target_link_libraries(IMUKit
	CoreIMU
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
	)
endif()
