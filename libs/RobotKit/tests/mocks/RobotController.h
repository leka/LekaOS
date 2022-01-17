// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_ROBOT_KIT_TESTS_MOCK_ROBOT_CONTROLLER_H_
#define _LEKA_OS_LIB_ROBOT_KIT_TESTS_MOCK_ROBOT_CONTROLLER_H_

#include "gmock/gmock.h"
#include "interface/RobotController.h"

namespace leka::mock {

struct RobotController : public interface::RobotController {
  public:
	MOCK_METHOD(void, startSystem, (), (override));
	MOCK_METHOD(void, stopSystem, (), (override));

	MOCK_METHOD(void, onRunningEntry, (), (override));
};

}	// namespace leka::mock

#endif	 // _LEKA_OS_LIB_ROBOT_KIT_TESTS_MOCK_ROBOT_CONTROLLER_H_
