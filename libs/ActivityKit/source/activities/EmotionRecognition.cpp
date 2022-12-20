// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "EmotionRecognition.h"
#include <random>

#include "rtos/ThisThread.h"
namespace leka::activity {

void EmotionRecognition::start()
{
	_current_round	 = 0;
	_current_emotion = {};

	_backup_callback = _rfidkit.getCallback();
	std::shuffle(_emotions.begin(), _emotions.end(), std::mt19937(static_cast<unsigned int>(time(nullptr))));
	launchNextRound();

	_rfidkit.onTagActivated([this](const MagicCard &card) { processCard(card); });
}

void EmotionRecognition::stop()
{
	_rfidkit.onTagActivated(_backup_callback);
}

void EmotionRecognition::processCard(const MagicCard &card)
{
	using namespace std::chrono;

	if (card == std::get<0>(_current_emotion->cards) || card == std::get<1>(_current_emotion->cards)) {
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

void EmotionRecognition::launchNextRound()
{
	_current_emotion = _emotions.at(_current_round);

	auto full_path = "/fs/home/img/id/" + std::string(_current_emotion->id) + ".jpg";
	_videokit.displayImage(full_path);
}

}	// namespace leka::activity

// LCOV_EXCL_STOP
