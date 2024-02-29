// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, registerEvents)
{
	EXPECT_CALL(battery, onDataUpdated);
	EXPECT_CALL(battery, onLowBattery);
	EXPECT_CALL(battery, startEventHandler);
	EXPECT_CALL(battery, onChargeDidStart);
	EXPECT_CALL(battery, onChargeDidStop);

	rc.registerEvents();

	// TODO: Expect raise call
}
