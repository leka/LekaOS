// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "ShapeRecognition.h"
#include <random>

#include "rtos/ThisThread.h"

namespace leka::activity {

void ShapeRecognition::start()
{
	_score		   = 0;
	_current_shape = {};

	_backup_callback = _rfidkit.getCallback();
	std::shuffle(_shapes.begin(), _shapes.end(), std::mt19937(static_cast<unsigned int>(time(nullptr))));
	launchNextRound();

	_rfidkit.onTagActivated([this](const MagicCard &card) { processCard(card); });
}

void ShapeRecognition::stop()
{
	_rfidkit.onTagActivated(_backup_callback);
}

void ShapeRecognition::processCard(const MagicCard &card)
{
	using namespace std::chrono;

	if (card == _current_shape->card) {
		_reinforcerkit.playDefault();
		rtos::ThisThread::sleep_for(5s);
		++_score;

		if (_score == kRoundsNumber) {
			_backup_callback(MagicCard::dice_roll);
			return;
		}

		launchNextRound();
	} else {
		_backup_callback(card);
	}
};

void ShapeRecognition::launchNextRound()
{
	_current_shape = _shapes.at(_score);

	auto full_path = "/fs/home/img/id/" + std::string(_current_shape->id) + ".jpg";
	_videokit.fillWhiteBackgroundAndDisplayImage(full_path);
}

}	// namespace leka::activity

// LCOV_EXCL_STOP
