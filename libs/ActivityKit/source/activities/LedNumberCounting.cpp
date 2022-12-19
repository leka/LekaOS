// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "LedNumberCounting.h"
#include <random>

#include "rtos/ThisThread.h"

namespace leka::activity {

void LedNumberCounting::start()
{
	_current_round		 = 0;
	_current_leds_number = 0;
	_expected_tag_number = MagicCard::none;

	_videokit.displayImage("fs/home/img/system/robot-face-smiling-slightly.jpg");

	_backup_callback = _rfidkit.getCallback();
	std::shuffle(_led_indexes.begin(), _led_indexes.end(), std::mt19937(static_cast<unsigned int>(time(nullptr))));
	std::shuffle(_led_numbers.begin(), _led_numbers.end(), std::mt19937(static_cast<unsigned int>(time(nullptr))));
	launchNextRound();

	_rfidkit.onTagActivated([this](const MagicCard &card) { processCard(card); });
}

void LedNumberCounting::stop()
{
	_rfidkit.onTagActivated(_backup_callback);
}

void LedNumberCounting::processCard(const MagicCard &card)
{
	using namespace std::chrono;

	if (card == _expected_tag_number) {
		_reinforcerkit.playDefault();
		rtos::ThisThread::sleep_for(6s);
		++_current_round;

		if (_current_round == kRoundsNumber) {
			_backup_callback(MagicCard::dice_roll);
			return;
		}

		std::shuffle(_led_indexes.begin(), _led_indexes.end(), std::mt19937(static_cast<unsigned int>(time(nullptr))));
		launchNextRound();
	} else {
		_backup_callback(card);
	}
}

void LedNumberCounting::launchNextRound()
{
	_current_leds_number = _led_numbers.at(_current_round);
	_expected_tag_number = MagicCard(MagicCard::number_0.getId() + _current_leds_number);

	for (auto i = 0; i < _current_leds_number; ++i) {
		_led.setColorAtIndex(_led_indexes.at(i), colors_table.at(i));
	}
	_led.show();
}

}	// namespace leka::activity

// LCOV_EXCL_STOP
