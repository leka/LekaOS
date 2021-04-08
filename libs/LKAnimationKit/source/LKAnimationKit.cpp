// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKAnimationKit.h"

#include "rtos/ThisThread.h"

using namespace std::chrono;
using namespace leka;

LKAnimationKit::LKAnimationKit(rtos::Thread &thread, interface::CGAnimation &animation)
	: _thread(thread), _animation(animation)
{
	_thread.start({this, &LKAnimationKit::runner});
}

void LKAnimationKit::start(interface::CGAnimation &animation)
{
	stop();

	_animation = animation;
	_animation.start();

	_run_animation = true;
	while (_run_animation) {
		rtos::ThisThread::sleep_for(100ms);
	}
}

void LKAnimationKit::stop()
{
	_animation.stop();
}

__attribute__((noreturn)) void LKAnimationKit::runner()
{
	while (true) {
		if (_run_animation) {
			_run_animation = false;
			_animation.run();
		}

		rtos::ThisThread::sleep_for(1s);
	}
}
