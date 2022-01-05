// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "EventFlags.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "StateMachine.h"

using namespace leka;
using namespace std::chrono;

auto event_flags_main_state_machine = rtos::EventFlags {};
auto state_machine					= Robot {event_flags_main_state_machine};

auto main() -> int
{
	logger::init();

	rtos::ThisThread::sleep_for(1s);

	log_info("\n\n");
	log_info("Hello, LekaOS!\n");

	rtos::ThisThread::sleep_for(2s);

	auto hello = HelloWorld();
	hello.start();

	state_machine.StateLoop();

	while (true) {
		log_debug("A message from your board %s --> \"%s\" at %ims", MBED_CONF_APP_TARGET_NAME, hello.world,
				  int(rtos::Kernel::Clock::now().time_since_epoch().count()));
		rtos::ThisThread::sleep_for(1s);
	}
}
