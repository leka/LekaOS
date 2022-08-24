// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "NumberRecognition.h"

namespace leka::rfid::activity {

auto NumberRecognition::playReinforcer() -> bool
{
	return _play_reinforcer;
}

void NumberRecognition::setUtils(ReinforcerKit &reinforcerkit, interface::VideoKit &videokit, interface::LED &led)
{
	_reinforcerkit = &reinforcerkit;
	_videokit	   = &videokit;
	_led		   = &led;
}

void NumberRecognition::registerCallback(const std::function<void(const MagicCard &card)> &callback)
{
	// do nothing
}

void NumberRecognition::start()
{
	uint8_t pos					  = 0;
	uint8_t random_emotion		  = 0;
	MagicCard expected_tag_number = MagicCard::none;

	last_numbers_displayed.push(-1);
	_play_reinforcer = false;

	setRandomNumberDisplay();
}

void NumberRecognition::stop()
{
	// _behaviorkit->stop();
	_play_reinforcer = false;
}

void NumberRecognition::run(const MagicCard &card)
{
	if (card == expected_tag_number) {
		_play_reinforcer = true;
	} else {
		setRandomNumberDisplay();
	}
}

void NumberRecognition::setRandomNumberDisplay()
{
	do {
		random_number = utils::math::random8(0, numbers_table.size());	 // TODO shuffle
		pos			  = 0;
	} while (last_numbers_displayed.hasPattern(&random_number, 1, pos));
	last_numbers_displayed.push(random_number);

	last_image			= numbers_table.at(random_number);
	expected_tag_number = MagicCard(MagicCard::number_0.getId() + random_number);

	std::string full_path =
		"/fs/home/img/" + last_image + ".jpg";	 // TODO (@hugo) Add real path when pictos are added to fs
	_videokit->displayImage(full_path);
}

}	// namespace leka::rfid::activity

// LCOV_EXCL_STOP
