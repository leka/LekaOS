// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "DesignPattern.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono;

void DesignPattern::start()
{
	auto isTouched_func	 = [&](const Position position) { isTouched(position); };
	auto isReleased_func = [&](const Position position) { isReleased(position); };

	_touch_sensor_kit.init();
	_touch_sensor_kit.registerOnSensorTouched(isTouched_func);
	_touch_sensor_kit.registerOnSensorReleased(isReleased_func);
	_touch_sensor_kit.start();

	_running = true;

	rtos::ThisThread::sleep_for(1s);
}

void DesignPattern::run()
{
	switch (_step) {
		case Step::LAUNCH:
			for (_iterator = 0; _iterator < _pattern.size(); ++_iterator) {
				_pattern.at(_iterator) =
					static_cast<Position>(std::rand() / ((RAND_MAX) / TouchSensorKit::kNumberOfSensors));
				_led_manager.turnOnComponent(_pattern.at(_iterator), _start_color);
				rtos::ThisThread::sleep_for(1s);
				_led_manager.turnOffComponent(_pattern.at(_iterator));
				rtos::ThisThread::sleep_for(1s);
			}
			_led_manager.playReinforcer();
			rtos::ThisThread::sleep_for(3s);
			_iterator = 0;
			_step	  = Step::PLAY;
			break;
		case Step::PLAY:
			if (_iterator < _pattern.size()) {
				auto position = _pattern.at(_iterator);
				if (_state.at(static_cast<size_t>(position))) {
					_led_manager.turnOnComponent(position, _win_color);
					rtos::ThisThread::sleep_for(1s);
					_led_manager.turnOffComponent(position);
					while (!_state.at(static_cast<size_t>(position))) {
					}
					++_iterator;
				}
			} else {
				_step = Step::WIN;
			}
			break;
		case Step::WIN:
			_led_manager.playReinforcer();
			rtos::ThisThread::sleep_for(3s);
			break;

		default:
			break;
	}
}

void DesignPattern::stop()
{
	_running = false;
	_led_manager.stop();
	_touch_sensor_kit.stop();
}

auto DesignPattern::isRunning() const -> bool
{
	return _running;
}

void DesignPattern::isTouched(const Position position)
{
	_state.at(static_cast<size_t>(position)) = true;
}

void DesignPattern::isReleased(const Position position)
{
	_state.at(static_cast<size_t>(position)) = false;
}
