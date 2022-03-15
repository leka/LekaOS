// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "boost/sml.hpp"

namespace leka::interface {

class RobotController
{
  public:
	virtual ~RobotController() = default;

	virtual void runLaunchingBehavior() = 0;

	virtual void startSleepTimeout() = 0;
	virtual void stopSleepTimeout()	 = 0;

	virtual void startWaitingBehavior() = 0;
	virtual void stopWaitingBehavior()	= 0;

	virtual void startSleepingBehavior() = 0;
	virtual void stopSleepingBehavior()	 = 0;

	virtual auto isCharging() -> bool = 0;

	virtual void startChargingBehavior() = 0;
	virtual void stopChargingBehavior()	 = 0;

	virtual auto isReadyToUpdate() -> bool = 0;
	virtual void applyUpdate()			   = 0;
};

}	// namespace leka::interface
