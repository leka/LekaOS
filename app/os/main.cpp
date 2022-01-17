// Leka - LekaOS
// Copyright 2020-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "HelloWorld.h"
#include "LogKit.h"
#include "RobotController.h"
#include "StateMachine.h"

using namespace leka;
using namespace std::chrono;

auto thread_rc = rtos::Thread {};

auto rc = RobotController {};
auto sm = boost::sml::sm<system::robot::StateMachine> {static_cast<interface::RobotController &>(rc)};

auto main() -> int
{
	logger::init();

	rtos::ThisThread::sleep_for(1s);

	log_info("\n\n");
	log_info("Hello, LekaOS!\n");

	rtos::ThisThread::sleep_for(2s);

	auto hello = HelloWorld();
	hello.start();

	rc.setStateMachine(&sm);
	thread_rc.start({&rc, &RobotController::startMainLoop});

	while (true) {
		log_debug("A message from your board %s --> \"%s\" at %ims", MBED_CONF_APP_TARGET_NAME, hello.world,
				  int(rtos::Kernel::Clock::now().time_since_epoch().count()));
		rtos::ThisThread::sleep_for(1s);
	}
}
