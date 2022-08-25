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
	GameKit
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
		tests/StateMachine_test.cpp
		tests/RobotController_test_initializeComponents.cpp
		tests/RobotController_test_registerEvents.cpp
		tests/RobotController_test_stateSetup.cpp
		tests/RobotController_test_stateIdle.cpp
		tests/RobotController_test_stateSleeping.cpp
		tests/RobotController_test_stateCharging.cpp
		tests/RobotController_test_stateConnected.cpp
		tests/RobotController_test_stateDisconnected.cpp
		tests/RobotController_test_stateWorking.cpp
		tests/RobotController_test_stateEmergencyStopped.cpp
	)
endif()
