// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_ROBOT_CONTROLLER_H_
#define _LEKA_OS_ROBOT_CONTROLLER_H_

#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "BatteryKit.h"
#include "CoreBattery.h"
#include "EventQueue.h"
#include "StateMachine.h"
#include "Ticker.h"
#include "interface/RobotController.h"

namespace leka {

// class Timeout
// {
//   public:
// 	Timeout() { thread.start({this, &Timeout::countdown}); }

// 	void registerCallback(mbed::Callback<void()> c) { callback = c; }

// 	void countdown()
// 	{
// 		start = rtos::Kernel::Clock::now();
// 		while (true) {
// 			auto diff = rtos::Kernel::Clock::now() - start;
// 			if (diff.count() > 5000) {
// 				// callback();
// 			}
// 			rtos::ThisThread::sleep_for(100);
// 			// do nothing
// 		}
// 	}

//   private:
// 	rtos::Thread thread;
// 	mbed::Callback<void()> callback;
// 	struct std::chrono::time_point<rtos::Kernel::Clock, std::chrono::duration<long long, std::ratio<1, 1000>>> start;
// };

class RobotController : public interface::RobotController
{
  public:
	RobotController() = default;

	// void start();

	// void setCallbacks();

	void wakeupSystem() final;
	void fallAsleepSystem() final;

	void onEntryWaitingForCommands() final;

	// template <typename eventToProcess>
	// void processEvent(eventToProcess event);

	// void processTimeout();
	// void process(system::robot::sm::event::timeout e);

	// template <typename T>
	// void process();

  private:
	boost::sml::sm<system::robot::StateMachine> _sm {static_cast<interface::RobotController &>(*this)};

	// Timeout timeout;

	// rtos::Thread t {};
	// events::EventQueue eq {};

	// mbed::Callback<void()> a;

	// std::function<void(void *)> _lambda;
};

}	// namespace leka

#endif	 // _LEKA_OS_ROBOT_CONTROLLER_H_
