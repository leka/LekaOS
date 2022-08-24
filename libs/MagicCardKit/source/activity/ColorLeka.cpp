// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "ColorLeka.h"

namespace leka::rfid::activity {

auto ColorLeka::playReinforcer() -> bool
{
	return _play_reinforcer;
}

void ColorLeka::setUtils(ReinforcerKit &reinforcerkit, interface::VideoKit &videokit, interface::LED &led)
{
	_reinforcerkit = &reinforcerkit;
	_videokit	   = &videokit;
	_led		   = &led;
}

void ColorLeka::registerCallback(const std::function<void(const MagicCard &card)> &callback)
{
	// do nothing
}

void ColorLeka::start()
{
	_videokit->displayImage("path");   // TODO neutral here
	_play_reinforcer = false;
}

void ColorLeka::stop()
{
	// _behaviorkit->stop();
	_play_reinforcer = false;
}

void ColorLeka::run(const MagicCard &card)
{
	using namespace std::chrono_literals;

	switch (card.getId()) {
		case MagicCard::color_red.getId():
			_videokit->playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi");
			_led->setColor(RGB::pure_red);
			rtos::ThisThread::sleep_for(4s);
			break;
		case MagicCard::color_green.getId():
			_videokit->playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi");
			_led->setColor(RGB::pure_green);
			rtos::ThisThread::sleep_for(4s);
			break;
		case MagicCard::color_blue.getId():
			_videokit->playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi");
			_led->setColor(RGB::pure_blue);
			rtos::ThisThread::sleep_for(4s);
			break;
		case MagicCard::color_yellow.getId():
			_videokit->playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi");
			_led->setColor(RGB::yellow);
			rtos::ThisThread::sleep_for(4s);
			break;
		case MagicCard::color_purple.getId():
			_videokit->playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi");
			_led->setColor(RGB::magenta);
			rtos::ThisThread::sleep_for(4s);
			break;
		case MagicCard::color_orange.getId():
			_videokit->playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi");
			_led->setColor(RGB {255, 126, 0});
			rtos::ThisThread::sleep_for(4s);
			break;
		default:
			break;
	}
	_videokit->displayImage("path");   // TODO neutral here
	_led->setColor(RGB::black);
}

}	// namespace leka::rfid::activity

// LCOV_EXCL_STOP
