// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_ROBOT_CONTROLLER_H_
#define _LEKA_OS_ROBOT_CONTROLLER_H_

#include "StateMachine.h"
#include "interface/RobotController.h"

namespace leka {

template <typename sm_t = boost::sml::sm<system::robot::StateMachine>>
class RobotController : public interface::RobotController
{
  public:
	sm_t state_machine {static_cast<interface::RobotController &>(*this)};

	RobotController() = default;

	void startSystem() final {};
	void stopSystem() final {};
};

}	// namespace leka

#endif	 // _LEKA_OS_ROBOT_CONTROLLER_H_
