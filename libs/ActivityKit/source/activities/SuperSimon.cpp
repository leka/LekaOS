// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include <random>

#include "rtos/ThisThread.h"

#include "SuperSimon.h"

namespace leka::activity {

void SuperSimon::start()
{
	using namespace std::chrono;

	_current_round		  = 0;
	_expected_color_index = 0;

	_videokit.displayImage("system/robot-face-smiling-slightly.jpg");

	_backup_callback = _rfidkit.getCallback();
	std::shuffle(_colors.begin(), _colors.end(), std::mt19937(static_cast<unsigned int>(time(nullptr))));
	rtos::ThisThread::sleep_for(2s);

	launchNextRound();

	_rfidkit.onTagActivated([this](const MagicCard &card) { processCard(card); });
}

void SuperSimon::stop()
{
	_rfidkit.onTagActivated(_backup_callback);
}

void SuperSimon::processCard(const MagicCard &card)
{
	using namespace std::chrono;

	if (card == _expected_colors.at(_expected_color_index)->card) {
		_led.setColor(_expected_colors.at(_expected_color_index)->color);
		_led.show();
		rtos::ThisThread::sleep_for(400ms);

		_led.setColor(RGB::black);
		_led.show();
		rtos::ThisThread::sleep_for(500ms);

		if (_expected_color_index == _current_round) {
			_reinforcerkit.playDefault();
			rtos::ThisThread::sleep_for(5s);
			_expected_color_index = 0;
			++_current_round;

			if (_current_round == _last_round) {
				_backup_callback(MagicCard::dice_roll);
				return;
			}

			launchNextRound();
			return;
		}

		++_expected_color_index;

	} else {
		_backup_callback(card);
	}
};

void SuperSimon::launchNextRound()
{
	using namespace std::chrono;

	for (auto i = 0; i <= _current_round; ++i) {
		_expected_colors.at(i) = _colors.at(i);

		_led.setColor(_expected_colors.at(i)->color);
		_led.show();
		rtos::ThisThread::sleep_for(400ms);

		_led.setColor(RGB::black);
		_led.show();
		rtos::ThisThread::sleep_for(500ms);
	}
}

}	// namespace leka::activity

// LCOV_EXCL_STOP
