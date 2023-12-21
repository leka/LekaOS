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
	FileManagerKit
	CoreMutex
	VideoKit
	BehaviorKit
	CommandKit
	RFIDKit
	ActivityKit
	ConfigKit
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/StateMachine_test.cpp
	)

	if(NOT ${UT_LITE})
		leka_unit_tests_sources(
			tests/RobotController_test_initializeComponents.cpp
			tests/RobotController_test_registerEvents.cpp
			tests/RobotController_test_stateConnected.cpp
			tests/RobotController_test_stateDisconnected.cpp
		)
	endif()
endif()
