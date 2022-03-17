# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(RobotKit STATIC)

target_include_directories(RobotKit
	PUBLIC
		include
	PRIVATE
		tests
)

target_sources(RobotKit
	PRIVATE
		source/StateMachine.cpp
		source/RobotController.cpp
)

target_link_libraries(RobotKit
	BatteryKit
	SerialNumberKit
	BLEKit
	CoreMotor
	LedKit
	CoreMutex
	VideoKit
	BehaviorKit
	CommandKit
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/StateMachine_test.cpp
		tests/RobotController_test.cpp
	)

endif()
