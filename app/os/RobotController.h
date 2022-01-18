// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_ROBOT_CONTROLLER_H_
#define _LEKA_OS_ROBOT_CONTROLLER_H_

#include "StateMachine.h"
#include "interface/RobotController.h"

namespace leka {

class RobotController : public interface::RobotController
{
  public:
	RobotController() = default;

	void wakeupSystem() final;
	void fallAsleepSystem() final;

	void onEntryWaitingForCommands() final;

  private:
	boost::sml::sm<system::robot::StateMachine> _sm {static_cast<interface::RobotController &>(*this)};
};

}	// namespace leka

#endif	 // _LEKA_OS_ROBOT_CONTROLLER_H_
