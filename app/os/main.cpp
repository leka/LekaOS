// Leka - LekaOS
// Copyright 2020-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/EventFlags.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"

#include "HelloWorld.h"
#include "LogKit.h"
#include "RobotController.h"
#include "StateMachine.h"

using namespace leka;
using namespace std::chrono;

auto event_flags_rc = rtos::EventFlags {};

auto rc = RobotController {event_flags_rc};
auto sm = boost::sml::sm<system::robot::StateMachine> {static_cast<interface::RobotController &>(rc)};

auto timeout_flag_is_set = []() {
	return (event_flags_rc.get() & RobotController::Flags::TIMEOUT) == RobotController::Flags::TIMEOUT;
};

auto start_flag_is_set = []() {
	return (event_flags_rc.get() & RobotController::Flags::START) == RobotController::Flags::START;
};

auto main() -> int
{
	logger::init();

	rtos::ThisThread::sleep_for(1s);

	log_info("\n\n");
	log_info("Hello, LekaOS!\n");

	rtos::ThisThread::sleep_for(2s);

	auto hello = HelloWorld();
	hello.start();

	rc.startEventQueueDispatch();
	sm.process_event(leka::system::robot::sm::event::start {});

	while (true) {
		event_flags_rc.wait_any(RobotController::Flags::ALL, osWaitForever, false);

		if (timeout_flag_is_set()) {
			sm.process_event(system::robot::sm::event::timeout {});

		} else if (start_flag_is_set()) {
			sm.process_event(system::robot::sm::event::start {});
		}

		event_flags_rc.clear(RobotController::Flags::ALL);
	}
}
