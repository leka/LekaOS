// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "ColorRecognition.h"

namespace leka::rfid::activity {

auto ColorRecognition::playReinforcer() -> bool
{
	return _play_reinforcer;
}

void ColorRecognition::setUtils(ReinforcerKit &reinforcerkit, interface::VideoKit &videokit, interface::LED &led)
{
	_reinforcerkit = &reinforcerkit;
	_videokit	   = &videokit;
	_led		   = &led;
}

void ColorRecognition::registerCallback(const std::function<void(const MagicCard &card)> &callback)
{
	// do nothing
}

void ColorRecognition::start()
{
	uint8_t pos					 = 0;
	uint8_t random_color		 = 0;
	uint8_t random_family		 = 0;
	std::string last_image		 = "NaN";
	MagicCard expected_tag_color = MagicCard::none;

	last_color_displayed.push(-1);
	last_family_displayed.push(-1);

	_play_reinforcer = false;
	setRandomColorAndFamilyDisplay();
}

void ColorRecognition::stop()
{
	// _behaviorkit->stop();
	_play_reinforcer = false;
}

void ColorRecognition::run(const MagicCard &card)
{
	if (card == expected_tag_color) {
		_play_reinforcer = true;
	} else {
		setRandomColorAndFamilyDisplay();
	}
}

void ColorRecognition::setRandomColorAndFamilyDisplay()
{
	do {
		random_color = utils::math::random8(0, number_of_different_colors);
		pos			 = 0;
	} while (last_color_displayed.hasPattern(&random_color, 1, pos));
	last_color_displayed.push(random_color);

	do {
		random_family = utils::math::random8(0, object_colored_family_table.size());
		pos			  = 0;
	} while (last_family_displayed.hasPattern(&random_family, 1, pos));
	last_family_displayed.push(random_family);

	switch (random_color) {
		case 0:
			last_image = "red";	  // TODO (@hugo) find solution object_colored_family_table.at(random_family) + "-red";
			expected_tag_color = MagicCard::color_red;
			break;
		case 1:
			last_image =
				"blue";	  // TODO (@hugo) find solution object_colored_family_table.at(random_family) + "-blue";
			expected_tag_color = MagicCard::color_blue;
			break;
		case 2:
			last_image =
				"green";   // TODO (@hugo) find solution object_colored_family_table.at(random_family) + "-green";
			expected_tag_color = MagicCard::color_green;
			break;
		case 3:
			last_image =
				"yellow";	// TODO (@hugo) find solution object_colored_family_table.at(random_family) + "-yellow";
			expected_tag_color = MagicCard::color_yellow;
			break;
		default:
			break;
	}
	std::string full_path =
		"/fs/home/img/" + last_image + ".jpg";	 // TODO (@hugo) Add real path when pictos are added to fs
	_videokit->displayImage(full_path);
}
}	// namespace leka::rfid::activity

// LCOV_EXCL_STOP
