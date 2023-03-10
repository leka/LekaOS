// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "NumberRecognition.h"
#include <random>

#include "rtos/ThisThread.h"

namespace leka::activity {

void NumberRecognition::start(const std::function<void()> &before_process_callback)
{
	_current_round	= 0;
	_current_number = {};

	_backup_callback = _rfidkit.getCallback();
	std::shuffle(_numbers.begin(), _numbers.end(), std::mt19937(static_cast<unsigned int>(time(nullptr))));
	launchNextRound();

	_rfidkit.onTagActivated([this, &before_process_callback](const MagicCard &card) {
		before_process_callback();
		processCard(card);
	});
}

void NumberRecognition::stop()
{
	_rfidkit.onTagActivated(_backup_callback);
}

void NumberRecognition::processCard(const MagicCard &card)
{
	using namespace std::chrono;

	if (card == _current_number->card) {
		_reinforcerkit.playDefault();
		rtos::ThisThread::sleep_for(5s);
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

void NumberRecognition::launchNextRound()
{
	_current_number = _numbers.at(_current_round);

	auto full_path = "/fs/home/img/id/" + std::string(_current_number->id) + ".jpg";
	_videokit.fillWhiteBackgroundAndDisplayImage(full_path);
}

}	// namespace leka::activity

// LCOV_EXCL_STOP
