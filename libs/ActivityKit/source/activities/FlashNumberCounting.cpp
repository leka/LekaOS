// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "FlashNumberCounting.h"
#include <random>

#include "rtos/ThisThread.h"

namespace leka::activity {

void FlashNumberCounting::start(const std::function<void()> &before_process_callback)
{
	using namespace std::chrono;

	_current_round			= 0;
	_current_flashes_number = 0;
	_expected_tag_number	= MagicCard::none;

	_videokit.displayImage("fs/home/img/system/robot-face-smiling-slightly.jpg");

	_backup_callback = _rfidkit.getCallback();
	std::shuffle(_flash_numbers.begin(), _flash_numbers.end(), std::mt19937(static_cast<unsigned int>(time(nullptr))));
	rtos::ThisThread::sleep_for(2s);

	launchNextRound();

	_rfidkit.onTagActivated([this, &before_process_callback](const MagicCard &card) {
		if (before_process_callback != nullptr) {
			before_process_callback();
		}
		processCard(card);
	});
}

void FlashNumberCounting::stop()
{
	_rfidkit.onTagActivated(_backup_callback);
}

void FlashNumberCounting::processCard(const MagicCard &card)
{
	using namespace std::chrono;

	if (card == _expected_tag_number) {
		_reinforcerkit.playDefault();
		rtos::ThisThread::sleep_for(7s);

		++_current_round;

		if (_current_round == kRoundsNumber) {
			_backup_callback(MagicCard::dice_roll);
			return;
		}
		launchNextRound();
	} else {
		_backup_callback(card);
	}
}

void FlashNumberCounting::launchNextRound()
{
	using namespace std::chrono;

	_current_flashes_number = _flash_numbers.at(_current_round);
	_expected_tag_number	= MagicCard(MagicCard::number_0.getId() + _current_flashes_number);

	for (auto i = 0; i < _current_flashes_number; ++i) {
		_led.setColor(_colors.at(_current_round));
		_led.show();
		rtos::ThisThread::sleep_for(400ms);
		_led.setColor(RGB::black);
		_led.show();
		rtos::ThisThread::sleep_for(500ms);
	}
}

}	// namespace leka::activity

// LCOV_EXCL_STOP
