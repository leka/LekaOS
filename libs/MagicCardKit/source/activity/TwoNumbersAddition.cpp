// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "TwoNumbersAddition.h"

namespace leka::rfid::activity {

auto TwoNumbersAddition::playReinforcer() -> bool
{
	return _play_reinforcer;
}

void TwoNumbersAddition::setUtils(ReinforcerKit &reinforcerkit, interface::VideoKit &videokit, interface::LED &led)
{
	_reinforcerkit = &reinforcerkit;
	_videokit	   = &videokit;
	_led		   = &led;
}

void TwoNumbersAddition::registerCallback(const std::function<void(const MagicCard &card)> &callback)
{
	// do nothing
}

void TwoNumbersAddition::start()
{
	uint8_t pos					  = 0;
	uint8_t random_emotion		  = 0;
	MagicCard actual_tag_number_1 = MagicCard::none;
	MagicCard actual_tag_number_2 = MagicCard::none;

	last_numbers_displayed.push(-1);
	_play_reinforcer = false;
}

void TwoNumbersAddition::stop()
{
	// _behaviorkit->stop();
	_play_reinforcer = false;
}

void TwoNumbersAddition::run(const MagicCard &card)
{
	using namespace std::chrono;

	auto convertToRealNumber = [](MagicCard tag_number) { return tag_number.getId() - MagicCard::number_0.getId(); };

	if (!first_tag_valid && convertToRealNumber(actual_tag_number_1) >= 0 &&
		convertToRealNumber(actual_tag_number_1) < 10) {
		actual_tag_number_1 = card;
		_led->setColorRange(10, 19, RGB::white);
	} else if (first_tag_valid && convertToRealNumber(actual_tag_number_2) >= 0 &&
			   convertToRealNumber(actual_tag_number_2) < 10) {
		actual_tag_number_2 = card;
		_led->setColorRange(0, 9, RGB::white);
	}

	rtos::ThisThread::sleep_for(1s);

	auto sum = convertToRealNumber(actual_tag_number_1) + convertToRealNumber(actual_tag_number_2);

	if (sum == random_number) {
		_play_reinforcer = true;
	} else {
		setRandomNumberDisplay();
	}
}

void TwoNumbersAddition::setRandomNumberDisplay()
{
	do {
		random_number = utils::math::random8(0, numbers_table.size() - 1) + 1;
		pos			  = 0;
	} while (last_numbers_displayed.hasPattern(&random_number, 1, pos));
	last_numbers_displayed.push(random_number);

	last_image = numbers_table.at(random_number);
	_led->setColor(RGB::black);
	std::string full_path =
		"/fs/home/img/" + last_image + ".jpg";	 // TODO (@hugo) Add real path when pictos are added to fs
	_videokit->displayImage(full_path);
}

}	// namespace leka::rfid::activity

// LCOV_EXCL_STOP
