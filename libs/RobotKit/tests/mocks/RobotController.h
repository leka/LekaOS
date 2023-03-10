// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/RobotController.h"

namespace leka::mock {

struct RobotController : public interface::RobotController {
  public:
	MOCK_METHOD(void, runLaunchingBehavior, (), (override));

	MOCK_METHOD(void, startSleepTimeout, (int), (override));
	MOCK_METHOD(void, stopSleepTimeout, (), (override));

	MOCK_METHOD(void, startDeepSleepTimeout, (), (override));
	MOCK_METHOD(void, stopDeepSleepTimeout, (), (override));

	MOCK_METHOD(void, startIdleTimeout, (), (override));
	MOCK_METHOD(void, stopIdleTimeout, (), (override));

	MOCK_METHOD(void, startWaitingBehavior, (), (override));
	MOCK_METHOD(void, stopWaitingBehavior, (), (override));

	MOCK_METHOD(void, startSleepingBehavior, (), (override));
	MOCK_METHOD(void, stopSleepingBehavior, (), (override));

	MOCK_METHOD(void, startWorkingBehavior, (), (override));

	MOCK_METHOD(bool, isCharging, (), (override));
	MOCK_METHOD(bool, isBleConnected, (), (override));

	MOCK_METHOD(void, startChargingBehavior, (), (override));
	MOCK_METHOD(void, stopChargingBehavior, (), (override));

	MOCK_METHOD(void, startConnectionBehavior, (), (override));
	MOCK_METHOD(void, startDisconnectionBehavior, (), (override));

	MOCK_METHOD(void, startAutonomousActivityMode, (), (override));
	MOCK_METHOD(void, stopAutonomousActivityMode, (), (override));

	MOCK_METHOD(void, startFileExchange, (), (override));
	MOCK_METHOD(void, stopFileExchange, (), (override));
	MOCK_METHOD(bool, isReadyToFileExchange, (), (override));

	MOCK_METHOD(bool, isReadyToUpdate, (), (override));
	MOCK_METHOD(void, applyUpdate, (), (override));

	MOCK_METHOD(void, stopActuatorsAndLcd, (), (override));
	MOCK_METHOD(void, suspendHardwareForDeepSleep, (), (override));

	MOCK_METHOD(void, resetEmergencyStopCounter, (), (override));
};

}	// namespace leka::mock
