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

	_event_loop.registerCallback([this] { run(); });
}

void LedKit::start(interface::LEDAnimation *animation)
{
	stop();

	_animation = animation;

	if (_animation == nullptr) {
		return;
	}

	_animation->setLeds(_ears, _belt);
	_animation->start();

	_event_loop.start();
}

void LedKit::run()
{
	while (_animation->isRunning()) {
		_animation->run();
		rtos::ThisThread::sleep_for(40ms);
	}
}

void LedKit::stop()
{
	_event_loop.stop();

	if (_animation != nullptr) {
		_animation->stop();
	}
}
