// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_ROBOT_CONTROLLER_H_
#define _LEKA_OS_ROBOT_CONTROLLER_H_

#include "events/EventQueue.h"
#include "rtos/Thread.h"

#include "StateMachine.h"
#include "interface/RobotController.h"

namespace leka {

class RobotController : public interface::RobotController
{
  public:
	RobotController() { _thread.start({&_event_queue, &events::EventQueue::dispatch_forever}); };

	void wakeupSystem() final;
	void fallAsleepSystem() final;

	void onEntryWaitingForCommands() final;

	template <typename T>
	void raise();

  private:
	boost::sml::sm<system::robot::StateMachine> _sm {static_cast<interface::RobotController &>(*this)};

	rtos::Thread _thread {};
	events::EventQueue _event_queue {};
};

}	// namespace leka

#endif	 // _LEKA_OS_ROBOT_CONTROLLER_H_
