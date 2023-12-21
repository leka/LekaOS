// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./RobotController_test.h"

TEST_F(RobotControllerTest, raise)
{
	rc.raise(system::robot::sm::event::setup_complete {});

	// nothing expected
}
