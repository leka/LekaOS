// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ReinforcerKit.h"

using namespace leka;
using namespace std::chrono;

static constexpr auto reinforcer_duration = 2700ms;

void ReinforcerKit::setDefaultReinforcer(const Reinforcer &reinforcer)
{
	_default_reinforcer = reinforcer;
}

void ReinforcerKit::play(const Reinforcer &reinforcer)
{
	switch (reinforcer) {
		case Reinforcer::Rainbow:
			playRainbow();
			break;
		case Reinforcer::BlinkGreen:
			playBlinkGreen();
			break;
		case Reinforcer::SpinBlink:
			playSpinBlink();
			break;
		case Reinforcer::Fire:
			playFire();
			break;
		case Reinforcer::Sprinkles:
			playSprinkles();
			break;
		default:
			playRainbow();
			break;
	}
}

void ReinforcerKit::playDefault()
{
	play(_default_reinforcer);
}

void ReinforcerKit::stop()
{
	_ledkit.stop();
	_motor_left.stop();
	_motor_right.stop();
	_videokit.stopVideo();
}

void ReinforcerKit::playBlinkGreen()
{
	_motor_left.spin(Rotation::clockwise, 1);
	_motor_right.spin(Rotation::clockwise, 1);
	_ledkit.start(&LedKit::animation::blink_green);
	_videokit.playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi");
	rtos::ThisThread::sleep_for(reinforcer_duration);
	_ledkit.stop();
	_motor_left.stop();
	_motor_right.stop();
}

void ReinforcerKit::playSpinBlink()
{
	_motor_left.spin(Rotation::counterClockwise, 1);
	_motor_right.spin(Rotation::counterClockwise, 1);
	_ledkit.start(&LedKit::animation::spin_blink);
	_videokit.playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi");
	rtos::ThisThread::sleep_for(reinforcer_duration);
	_ledkit.stop();
	_motor_left.stop();
	_motor_right.stop();
}

void ReinforcerKit::playFire()
{
	_ledkit.start(&LedKit::animation::fire);
	_videokit.playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi");
	rtos::ThisThread::sleep_for(reinforcer_duration);
	_ledkit.stop();
}

void ReinforcerKit::playSprinkles()
{
	_ledkit.start(&LedKit::animation::sprinkles);
	_videokit.playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi");
	rtos::ThisThread::sleep_for(reinforcer_duration);
	_ledkit.stop();
}

void ReinforcerKit::playRainbow()
{
	_ledkit.start(&LedKit::animation::rainbow);
	_videokit.playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi");
	rtos::ThisThread::sleep_for(reinforcer_duration);
	_ledkit.stop();
}
