// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LedKit.h"

#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono;

void LedKit::init()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();

	_thread.start(mbed::Callback(this, &LedKit::run));
}

void LedKit::start(interface::LEDAnimation *animation)
{
	stop();

	_animation = animation;

	if (_animation == nullptr) {
		return;
	}

	_event_flags.set(flags::START_LED_ANIMATION_FLAG);
}

void LedKit::initializeAnimation()
{
	_animation->setLeds(_ears, _belt);
	_animation->start();
}

void LedKit::runAnimation()
{
	_event_flags.clear(flags::STOP_LED_ANIMATION_FLAG);

	auto keep_running = [&]() {
		auto flags = _event_flags.get();
		return (flags != flags::STOP_LED_ANIMATION_FLAG);
	};
	while (keep_running() && _animation->isRunning()) {
		_animation->run();
		rtos::ThisThread::sleep_for(40ms);
	}
}

void LedKit::run()
{
	while (true) {
		_event_flags.wait_any(flags::START_LED_ANIMATION_FLAG);

		initializeAnimation();
		runAnimation();
	}
}

void LedKit::stop()
{
	_event_flags.set(flags::STOP_LED_ANIMATION_FLAG);

	if (_animation != nullptr) {
		_animation->stop();
	}
}
