// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "NumberRecognition.h"
#include <random>

namespace leka::game {

void NumberRecognition::start()
{
	using namespace std::chrono;

	uint8_t pos					  = 0;
	MagicCard expected_tag_number = MagicCard::none;

	_videokit.displayImage("fs/home/img/system/robot-misc-robot-misc-screen_empty_white.jpg");

	_backup_callback = _rfidkit.getCallback();
	std::shuffle(index_span.begin(), index_span.end(), std::mt19937());
	setNextDisplay();
	auto on_tag_detected_callback = [this](const MagicCard &card) {
		if (card == MagicCard::remote_standard) {
			stop();
		} else if (card == MagicCard::number_10) {
			_videokit.displayImage("fs/home/img/system/robot-misc-robot-misc-screen_empty_white.jpg");
			_reinforcerkit.runDefault();
			setNextDisplay();
		}
	};
	_rfidkit.onTagActivated(on_tag_detected_callback);
}

void NumberRecognition::stop()
{
	_rfidkit.onTagActivated(_backup_callback);
}

void NumberRecognition::setNextDisplay()
{
	number_index				 = index_table.at(pos);
	std::string displayed_number = numbers_table.at(number_index);

	expected_tag_number = MagicCard(MagicCard::number_0.getId() + number_index);

	auto full_path = "/fs/home/img/id/" + displayed_number + ".jpg";
	_videokit.displayImage(full_path);
	++pos;
}

}	// namespace leka::game

// LCOV_EXCL_STOP
