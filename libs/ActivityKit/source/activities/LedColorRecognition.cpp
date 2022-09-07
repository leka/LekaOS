// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "LedColorRecognition.h"

namespace leka::activity {

void LedColorRecognition::start()
{
	_current_round = 0;
	_current_color = {};

	_videokit.displayImage("fs/home/img/system/robot-face-smiling-slightly.jpg");

	_backup_callback = _rfidkit.getCallback();
	std::shuffle(_colors.begin(), _colors.end(), std::mt19937(static_cast<unsigned int>(time(nullptr))));
	launchNextRound();

	_rfidkit.onTagActivated([this](const MagicCard &card) { processCard(card); });
}

void LedColorRecognition::stop()
{
	_rfidkit.onTagActivated(_backup_callback);
}

void LedColorRecognition::processCard(const MagicCard &card)
{
	using namespace std::chrono;

	if (card == _current_color->card) {
		_reinforcerkit.playDefault();
		rtos::ThisThread::sleep_for(400ms);
		++_current_round;

		if (_current_round == kRoundsNumber) {
			_backup_callback(MagicCard::dice_roll);
			return;
		}

		launchNextRound();
	} else {
		_backup_callback(card);
	}
};

void LedColorRecognition::launchNextRound()
{
	_current_color = _colors.at(_current_round);

	_led.setColor(_current_color->color);
	_led.show();
}

}	// namespace leka::activity

// LCOV_EXCL_STOP
