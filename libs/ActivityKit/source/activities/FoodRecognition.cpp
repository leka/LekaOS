// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "FoodRecognition.h"
#include <random>

namespace leka::activity {

void FoodRecognition::start()
{
	_current_round = 0;
	_current_food  = {};

	_backup_callback = _rfidkit.getCallback();
	std::shuffle(_foods.begin(), _foods.end(), std::mt19937(static_cast<unsigned int>(time(nullptr))));
	launchNextRound();

	_rfidkit.onTagActivated([this](const MagicCard &card) { processCard(card); });
}

void FoodRecognition::stop()
{
	_rfidkit.onTagActivated(_backup_callback);
}

void FoodRecognition::processCard(const MagicCard &card)
{
	if (card == _current_food->card) {
		_reinforcerkit.playDefault();
		++_current_round;

		if (_current_round == kScoreToWin) {
			_backup_callback(MagicCard::dice_roll);
			return;
		}

		launchNextRound();
	} else {
		_backup_callback(card);
	}
}

void FoodRecognition::launchNextRound()
{
	_current_food = _foods.at(_current_round);

	auto full_path = "/fs/home/img/id/" + std::string(_current_food->id) + ".jpg";
	_videokit.fillWhiteBackgroundAndDisplayImage(full_path);
}

}	// namespace leka::activity

// LCOV_EXCL_STOP
