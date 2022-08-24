// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "ColoredShapeRecognition.h"

namespace leka::rfid::activity {

auto ColoredShapeRecognition::playReinforcer() -> bool
{
	return _play_reinforcer;
}

void ColoredShapeRecognition::setUtils(ReinforcerKit &reinforcerkit, interface::VideoKit &videokit, interface::LED &led)
{
	_reinforcerkit = &reinforcerkit;
	_videokit	   = &videokit;
	_led		   = &led;
}

void ColoredShapeRecognition::registerCallback(const std::function<void(const MagicCard &card)> &callback)
{
	// do nothing
}

void ColoredShapeRecognition::start()
{
	uint8_t pos			   = 0;
	uint8_t random_color   = 0;
	uint8_t random_shape   = 0;
	std::string last_image = "NaN";

	MagicCard expected_tag_color = MagicCard::none;
	MagicCard expected_tag_shape = MagicCard::none;
	MagicCard actual_tag_color	 = MagicCard::none;
	MagicCard actual_tag_shape	 = MagicCard::none;

	last_color_displayed.push(-1);
	last_shape_displayed.push(-1);
	_play_reinforcer = false;
}

void ColoredShapeRecognition::stop()
{
	// _behaviorkit->stop();
	_play_reinforcer = false;
}

void ColoredShapeRecognition::run(const MagicCard &card)
{
	using namespace std::chrono;

	auto tagIsAColor = [](MagicCard card) {
		return (card.getId() >= MagicCard::color_red.getId()) && (card.getId() <= MagicCard::color_yellow.getId());
	};
	auto tagIsAShape = [](MagicCard card) { return (card.getId() <= MagicCard::shape_star.getId()); };

	if (!shape_already_load && tagIsAShape(card)) {
		actual_tag_shape = card;
		_led->setColorRange(10, 19, RGB::white);
	} else if (!color_already_load && tagIsAColor(card)) {
		actual_tag_color = card;
		_led->setColorRange(0, 9, RGB::white);
	}

	rtos::ThisThread::sleep_for(1s);

	if ((expected_tag_color == actual_tag_color) && (expected_tag_shape == actual_tag_shape)) {
		_play_reinforcer = true;
	} else {
		setRandomColoredShapeDisplay();
	}
}

void ColoredShapeRecognition::setRandomColoredShapeDisplay()
{
	do {
		random_color = utils::math::random8(0, number_of_different_colors);
		pos			 = 0;
	} while (last_color_displayed.hasPattern(&random_color, 1, pos));
	last_color_displayed.push(random_color);

	do {
		random_shape = utils::math::random8(0, shapes_table.size());
		pos			 = 0;
	} while (last_shape_displayed.hasPattern(&random_shape, 1, pos));
	last_shape_displayed.push(random_shape);

	expected_tag_shape = MagicCard(random_shape + MagicCard::shape_circle.getId());
	switch (random_color) {
		case 0:
			last_image		   = "red";	  // TODO (@hugo) find solution shapes_table.at(random_shape) + "-red";
			expected_tag_color = MagicCard::color_red;
			break;
		case 1:
			last_image		   = "blue";   // TODO (@hugo) find solution shapes_table.at(random_shape) + "-blue";
			expected_tag_color = MagicCard::color_blue;
			break;
		case 2:
			last_image		   = "green";	// TODO (@hugo) find solution shapes_table.at(random_shape) + "-green";
			expected_tag_color = MagicCard::color_green;
			break;
		case 3:
			last_image		   = "yellow";	 // TODO (@hugo) find solution shapes_table.at(random_shape) + "-yellow";
			expected_tag_color = MagicCard::color_yellow;
			break;
		default:
			break;
	}
	std::string full_path =
		"/fs/home/img/" + last_image + ".jpg";	 // TODO (@hugo) Add real path when pictos are added to fs
	_videokit->displayImage(full_path);
	_led->setColor(RGB::black);
}
}	// namespace leka::rfid::activity

// LCOV_EXCL_STOP
