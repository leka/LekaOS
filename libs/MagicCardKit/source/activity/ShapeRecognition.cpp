// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "ShapeRecognition.h"

namespace leka::rfid::activity {

auto ShapeRecognition::playReinforcer() -> bool
{
	return _play_reinforcer;
}

void ShapeRecognition::setUtils(ReinforcerKit &reinforcerkit, interface::VideoKit &videokit, interface::LED &led)
{
	_reinforcerkit = &reinforcerkit;
	_videokit	   = &videokit;
	_led		   = &led;
}

void ShapeRecognition::registerCallback(const std::function<void(const MagicCard &card)> &callback)
{
	// do nothing
}

void ShapeRecognition::start()
{
	uint8_t pos					 = 0;
	uint8_t random_shape		 = 0;
	MagicCard expected_tag_shape = MagicCard::none;

	last_shapes_displayed.push(-1);
	_play_reinforcer = false;
}

void ShapeRecognition::stop()
{
	// _behaviorkit->stop();
	_play_reinforcer = false;
}

void ShapeRecognition::run(const MagicCard &card)
{
	if (card == expected_tag_shape) {
		_play_reinforcer = true;
	} else {
		setRandomShapeDisplay();
	}
}

void ShapeRecognition::setRandomShapeDisplay()
{
	do {
		random_shape = utils::math::random8(0, shapes_table.size());
		pos			 = 0;
	} while (last_shapes_displayed.hasPattern(&random_shape, 1, pos));
	last_shapes_displayed.push(random_shape);

	last_image		   = shapes_table.at(random_shape);
	expected_tag_shape = MagicCard(MagicCard::shape_circle.getId() + random_shape);
	std::string full_path =
		"/fs/home/img/" + last_image + ".jpg";	 // TODO (@hugo) Add real path when pictos are added to fs
	_videokit->displayImage(full_path);
}

}	// namespace leka::rfid::activity

// LCOV_EXCL_STOP
