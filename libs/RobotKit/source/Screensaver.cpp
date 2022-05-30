// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "Screensaver.h"

namespace leka {

void Screensaver::start()
{
	using namespace std::chrono_literals;
	// srand(static_cast<unsigned>(time(nullptr)));
	// static auto number_of_behavior_played		  = 0;
	// static constexpr auto kNumberOfBehaviorToPlay = 3;
	// uint8_t behaviorIndex						  = (rand() % 4);
	// uint8_t behaviorIndex = 0;

	_behaviorkit.underwater();
	rtos::ThisThread::sleep_for(40s);
	_behaviorkit.bubbles();
	rtos::ThisThread::sleep_for(40s);
	_behaviorkit.fly(_on_screensaver_ended);
	rtos::ThisThread::sleep_for(40s);
	// _behaviorkit.singing();
	// rtos::ThisThread::sleep_for(40s);

	// switch (behaviorIndex) {
	// 	case 0:
	// 		if (number_of_behavior_played < kNumberOfBehaviorToPlay - 1) {
	// 			_behaviorkit.singing([this] { start(); });
	// 		} else {
	// 			_behaviorkit.singing(_on_screensaver_ended);
	// 			number_of_behavior_played = 0;
	// 		}
	// 		++number_of_behavior_played;
	// 		break;
	// 	case 1:
	// 		if (number_of_behavior_played < kNumberOfBehaviorToPlay - 1) {
	// 			_behaviorkit.singing([this] { start(); });
	// 		} else {
	// 			_behaviorkit.singing(_on_screensaver_ended);
	// 			number_of_behavior_played = 0;
	// 		}
	// 		++number_of_behavior_played;
	// 		break;
	// 	case 2:
	// 		if (number_of_behavior_played < kNumberOfBehaviorToPlay - 1) {
	// 			_behaviorkit.singing([this] { start(); });
	// 		} else {
	// 			_behaviorkit.singing(_on_screensaver_ended);
	// 			number_of_behavior_played = 0;
	// 		}
	// 		++number_of_behavior_played;
	// 		break;
	// 	case 3:
	// 		if (number_of_behavior_played < kNumberOfBehaviorToPlay - 1) {
	// 			_behaviorkit.singing([this] { start(); });
	// 		} else {
	// 			_behaviorkit.singing(_on_screensaver_ended);
	// 			number_of_behavior_played = 0;
	// 		}
	// 		++number_of_behavior_played;
	// 		break;
	// 	default:
	// 		break;
	// }
	// ++behaviorIndex;
}

void Screensaver::stop()
{
	_behaviorkit.stop();
}

void Screensaver::onScreensaverEnded(std::function<void()> const &callback)
{
	_on_screensaver_ended = callback;
}

}	// namespace leka
