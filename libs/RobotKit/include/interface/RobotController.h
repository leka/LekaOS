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

	virtual void startSleepTimeout(int duration) = 0;
	virtual void stopSleepTimeout()				 = 0;

	virtual void startDeepSleepTimeout() = 0;
	virtual void stopDeepSleepTimeout()	 = 0;

	virtual void startIdleTimeout() = 0;
	virtual void stopIdleTimeout()	= 0;

	virtual void startWaitingBehavior() = 0;
	virtual void stopWaitingBehavior()	= 0;

	virtual void startSleepingBehavior() = 0;
	virtual void stopSleepingBehavior()	 = 0;

	virtual auto isCharging() -> bool	  = 0;
	virtual auto isBleConnected() -> bool = 0;

	virtual void startChargingBehavior() = 0;
	virtual void stopChargingBehavior()	 = 0;

	virtual void startConnectionBehavior()	  = 0;
	virtual void startDisconnectionBehavior() = 0;

	virtual void startAutonomousActivityMode() = 0;
	virtual void stopAutonomousActivityMode()  = 0;

	virtual void startWorkingBehavior() = 0;

	virtual void startFileExchange()			 = 0;
	virtual void stopFileExchange()				 = 0;
	virtual auto isReadyToFileExchange() -> bool = 0;

	virtual auto isReadyToUpdate() -> bool = 0;
	virtual void applyUpdate()			   = 0;

	virtual void stopActuatorsAndLcd()		   = 0;
	virtual void suspendHardwareForDeepSleep() = 0;

	virtual void resetEmergencyStopCounter() = 0;
};

}	// namespace leka::interface
