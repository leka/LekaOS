// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "EmotionRecognition.h"

namespace leka::rfid::activity {

auto EmotionRecognition::playReinforcer() -> bool
{
	return _play_reinforcer;
}

void EmotionRecognition::setUtils(ReinforcerKit &reinforcerkit, interface::VideoKit &videokit, interface::LED &led)
{
	_reinforcerkit = &reinforcerkit;
	_videokit	   = &videokit;
	_led		   = &led;
}

void EmotionRecognition::registerCallback(const std::function<void(const MagicCard &card)> &callback)
{
	// do nothing
}

void EmotionRecognition::start()
{
	uint8_t pos							 = 0;
	MagicCard expected_tag_emotion_child = MagicCard::none;
	MagicCard expected_tag_emotion_leka	 = MagicCard::none;
	uint8_t random_emotion				 = 0;
	last_emotions_displayed.push(-1);
	_play_reinforcer = false;

	setRandomEmotionDisplay();
}

void EmotionRecognition::stop()
{
	// _behaviorkit->stop();
	_play_reinforcer = false;
}

void EmotionRecognition::run(const MagicCard &card)
{
	if (card == expected_tag_emotion_child || card == expected_tag_emotion_leka) {
		_play_reinforcer = true;
	} else {
		setRandomEmotionDisplay();
	}
}

void EmotionRecognition::setRandomEmotionDisplay()
{
	do {
		random_emotion = utils::math::random8(0, emotions_table.size());
		pos			   = 0;
	} while (last_emotions_displayed.hasPattern(&random_emotion, 1, pos));
	last_emotions_displayed.push(random_emotion);

	last_image = emotions_table.at(random_emotion);

	switch (random_emotion) {
		case 0:
			expected_tag_emotion_child = MagicCard::emotion_joy_child;
			expected_tag_emotion_leka  = MagicCard::emotion_joy_leka;
			break;
		case 1:
			expected_tag_emotion_child = MagicCard::emotion_anger_child;
			expected_tag_emotion_leka  = MagicCard::emotion_anger_leka;
			break;
		case 2:
			expected_tag_emotion_child = MagicCard::emotion_fear_child;
			expected_tag_emotion_leka  = MagicCard::emotion_fear_leka;
			break;
		case 3:
			expected_tag_emotion_child = MagicCard::emotion_disgust_child;
			expected_tag_emotion_leka  = MagicCard::emotion_disgust_leka;
			break;
		case 4:
			expected_tag_emotion_child = MagicCard::emotion_sadness_child;
			expected_tag_emotion_leka  = MagicCard::emotion_sadness_leka;
			break;
		default:
			break;
	}
	std::string full_path =
		"/fs/home/img/system" + last_image + ".jpg";   // TODO (@hugo) Add real path when pictos are added to fs
	_videokit->displayImage(full_path);
}

}	// namespace leka::rfid::activity

// LCOV_EXCL_STOP
