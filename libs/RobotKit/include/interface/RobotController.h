// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_ROBOT_KIT_INTERFACE_ROBOT_CONTROLLER_H_
#define _LEKA_OS_LIB_ROBOT_KIT_INTERFACE_ROBOT_CONTROLLER_H_

#include "boost/sml.hpp"

namespace leka::interface {

class RobotController
{
  public:
	virtual ~RobotController() = default;

	virtual void wakeupSystem()		= 0;
	virtual void fallAsleepSystem() = 0;

	virtual void onEntryWaitingForCommands() = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_LIB_ROBOT_KIT_INTERFACE_ROBOT_CONTROLLER_H_
