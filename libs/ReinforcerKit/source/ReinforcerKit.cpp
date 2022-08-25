// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ReinforcerKit.h"

using namespace leka;
using namespace std::chrono;

static constexpr auto reinforcer_duration = 2700ms;

void ReinforcerKit::setDefaultReinforcer(const Reinforcer &reinforcer)
{
	default_reinforcer = reinforcer;
}

void ReinforcerKit::run(const Reinforcer &reinforcer)
{
	if (reinforcer == Reinforcer::BlinkGreen) {
		playBlinkGreen();
	} else if (reinforcer == Reinforcer::SpinBlink) {
		playSpinBlink();
	} else if (reinforcer == Reinforcer::Fire) {
		playFire();
	} else if (reinforcer == Reinforcer::Sprinkles) {
		playSprinkles();
	} else {
		playRainbow();
	}
}

void ReinforcerKit::runDefault()
{
	if (default_reinforcer == Reinforcer::BlinkGreen) {
		playBlinkGreen();
	} else if (default_reinforcer == Reinforcer::SpinBlink) {
		playSpinBlink();
	} else if (default_reinforcer == Reinforcer::Fire) {
		playFire();
	} else if (default_reinforcer == Reinforcer::Sprinkles) {
		playSprinkles();
	} else {
		playRainbow();
	}
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
