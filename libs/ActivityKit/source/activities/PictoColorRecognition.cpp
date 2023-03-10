// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "PictoColorRecognition.h"
#include <random>

#include "rtos/ThisThread.h"

namespace leka::activity {

void PictoColorRecognition::start(const std::function<void()> &before_process_callback)
{
	_current_round = 0;
	_current_color = {};

	_backup_callback = _rfidkit.getCallback();
	std::shuffle(_colors.begin(), _colors.end(), std::mt19937(static_cast<unsigned int>(time(nullptr))));
	launchNextRound();

	_rfidkit.onTagActivated([this, &before_process_callback](const MagicCard &card) {
		if (before_process_callback != nullptr) {
			before_process_callback();
		}
		processCard(card);
	});
}

void PictoColorRecognition::stop()
{
	_rfidkit.onTagActivated(_backup_callback);
}

void PictoColorRecognition::processCard(const MagicCard &card)
{
	using namespace std::chrono;

	if (card == _current_color->card) {
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

void PictoColorRecognition::launchNextRound()
{
	_current_color = _colors.at(_current_round);

	auto full_path = "/fs/home/img/id/" + std::string(_current_color->id) + ".jpg";
	_videokit.fillWhiteBackgroundAndDisplayImage(full_path);
}

}	// namespace leka::activity

// LCOV_EXCL_STOP
