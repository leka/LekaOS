// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "DisplayEmotions.h"

namespace leka::rfid::activity {

auto DisplayEmotions::playReinforcer() -> bool
{
	return _play_reinforcer;
}

void DisplayEmotions::setUtils(ReinforcerKit &reinforcerkit, interface::VideoKit &videokit, interface::LED &led)
{
	_reinforcerkit = &reinforcerkit;
	_videokit	   = &videokit;
	_led		   = &led;
}

void DisplayEmotions::registerCallback(const std::function<void(const MagicCard &card)> &callback)
{
	// do nothing
}

void DisplayEmotions::start()
{
	_videokit->displayImage("path");   // TODO neutral here
	_play_reinforcer = false;
}

void DisplayEmotions::stop()
{
	_play_reinforcer = false;
}

void DisplayEmotions::run(const MagicCard &card)
{
	switch (card.getId()) {
		case MagicCard::emotion_joy_leka.getId():
			_videokit->displayImage("fs/home/img/system/robot_face_happy.jpg");
			break;
		case MagicCard::emotion_sadness_leka.getId():
			_videokit->displayImage("fs/home/img/system/robot-face-sad-with_tears.jpg");
			break;
		case MagicCard::emotion_fear_leka.getId():
			_videokit->displayImage("fs/home/img/system/robot-face-afraid.jpg");
			break;
		case MagicCard::emotion_anger_leka.getId():
			_videokit->displayImage("fs/home/img/system/robot-face-angry.jpg");
			break;
		case MagicCard::emotion_disgust_leka.getId():
			_videokit->displayImage("fs/home/img/system/robot-face-disgusted.jpg");
			break;
		default:
			break;
	}
}

}	// namespace leka::rfid::activity

// LCOV_EXCL_STOP
