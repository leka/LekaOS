// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LedSync.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono;

void LedSync::start()
{
	auto turnOn_func  = [&](const Position position) { turnOn(position); };
	auto turnOff_func = [&](const Position position) { turnOff(position); };

	_touch_sensor_kit.init();
	_touch_sensor_kit.registerOnSensorTouched(turnOn_func);
	_touch_sensor_kit.registerOnSensorReleased(turnOff_func);
	_touch_sensor_kit.start();

	_running = true;

	rtos::ThisThread::sleep_for(1s);
}

void LedSync::run()
{
	rtos::ThisThread::sleep_for(1ms);
}

void LedSync::stop()
{
	_running = false;
	_led_manager.stop();
	_touch_sensor_kit.stop();
}

auto LedSync::isRunning() const -> bool
{
	return _running;
}

void LedSync::turnOn(const Position position)
{
	_led_manager.turnOnComponent(position, _color);
}

void LedSync::turnOff(const Position position)
{
	_led_manager.turnOffComponent(position);
}
