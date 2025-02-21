# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CommandKit STATIC)

target_include_directories(CommandKit
	PUBLIC
	include
)

target_sources(CommandKit
	PRIVATE
	source/CommandKit.cpp
)

target_link_libraries(CommandKit
	CoreEventQueue
	CoreLED
	CoreMotor
	ReinforcerKit
	BehaviorKit
	EventLoopKit
	mbed-os
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/CommandKit_test.cpp
		tests/CommandKit_test_push.cpp
		tests/CommandKit_test_register.cpp
		tests/CommandRunner_test.cpp
		tests/Command_Video_test.cpp
	)
endif()
