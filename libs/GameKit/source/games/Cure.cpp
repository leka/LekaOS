// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "Cure.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono;

void Cure::start()
{
	auto update_func = [&](const Position position) { update(position); };

	_touch_sensor_kit.init();
	_touch_sensor_kit.registerOnSensorTouched(update_func);
	_touch_sensor_kit.start();

	_running = true;

	rtos::ThisThread::sleep_for(1s);
}

void Cure::run()
{
	switch (_step) {
		case Step::START:
			for (auto &dot: _dots) {
				auto state = static_cast<State>(std::rand() / ((RAND_MAX) / 3U));
				dot		   = state;
			}
			_step = Step::PLAY;
			break;
		case Step::PLAY:
			setDots();
			if (_mobile_cursor_index < kNumberOfLedsBelt) {
				_belt.setColorAtIndex(_mobile_cursor_index, _mobile_cursor_color);
				_belt.show();
				rtos::ThisThread::sleep_for(2s);
				_mobile_cursor_index++;

			} else {
				_mobile_cursor_index = 0;
			}
			break;
		case Step::END:
			break;
		default:
			break;
	}

	rtos::ThisThread::sleep_for(1ms);
}

void Cure::stop()
{
	_running = false;
	_touch_sensor_kit.stop();
}

auto Cure::isRunning() const -> bool
{
	return _running;
}

void Cure::update(Position position)
{
	if ((position == Position::ear_left || position == Position::ear_right)) {
		switch (_dots.at(_mobile_cursor_index)) {
			case State::sick:
				_dots.at(_mobile_cursor_index) = State::cured;
				break;
			case State::forbidden:
				_step = Step::END;
				break;
			default:
				break;
		}
	}
}

void Cure::setDots()
{
	for (size_t index = 0; index < _dots.size(); index++) {
		switch (_dots.at(index)) {
			case State::base:
				_belt.setColorAtIndex(index, _base_fixed_dot_color);
				break;
			case State::sick:
				_belt.setColorAtIndex(index, _sick_fixed_dot_color);
				break;
			case State::forbidden:
				_belt.setColorAtIndex(index, _forbidden_fixed_dot_color);
				break;
			case State::cured:
				_belt.setColorAtIndex(index, _cured_fixed_dot_color);
				break;
			default:
				break;
		}
		_belt.show();
	}
}
