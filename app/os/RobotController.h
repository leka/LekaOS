// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_ROBOT_CONTROLLER_H_
#define _LEKA_OS_ROBOT_CONTROLLER_H_

#include "events/EventQueue.h"
#include "rtos/EventFlags.h"
#include "rtos/Thread.h"

#include "interface/RobotController.h"

namespace leka {

class RobotController : public interface::RobotController
{
  public:
	enum Flags : uint32_t
	{
		TIMEOUT = (1UL << 0),
		START	= (1UL << 1),
		ALL		= 0x7FFFFFFF
	};

	RobotController(rtos::EventFlags &event_flags) : _event_flags(event_flags) {}

	void startEventQueueDispatch();

	void startSystem() final;
	void stopSystem() final;

	void onRunningEntry() final;

  private:
	rtos::Thread _thread_event_queue {};
	events::EventQueue _event_queue {};

	rtos::EventFlags &_event_flags;
};

}	// namespace leka

#endif	 // _LEKA_OS_ROBOT_CONTROLLER_H_
