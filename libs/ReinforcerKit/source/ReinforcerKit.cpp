// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ReinforcerKit.h"

#include "LedKitAnimations.h"

using namespace leka;
using namespace std::chrono;

void ReinforcerKit::setDefaultReinforcer(const Reinforcer &reinforcer)
{
	_default_reinforcer = reinforcer;
}

void ReinforcerKit::play(const Reinforcer &reinforcer)
{
	switch (reinforcer) {
		case Reinforcer::Rainbow:
		default:
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
	}
}

void ReinforcerKit::playDefault()
{
	play(_default_reinforcer);
}

void ReinforcerKit::stop()
{
	_ledkit.stop();
	_audiokit.stop();
	_motionkit.stop();
	_videokit.stopVideo();
}

void ReinforcerKit::playBlinkGreen()
{
	_videokit.playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi");
	_ledkit.start(&led::animation::blink_green);
	_motionkit.startYawRotation(kThreeTurnDegrees, Rotation::clockwise, [this] { _ledkit.stop(); });
}

void ReinforcerKit::playSpinBlink()
{
	_videokit.playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi");
	_ledkit.start(&led::animation::spin_blink);
	_motionkit.startYawRotation(kThreeTurnDegrees, Rotation::counterClockwise, [this] { _ledkit.stop(); });
}

void ReinforcerKit::playFire()
{
	_videokit.playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi",
							[this] { _ledkit.stop(); });
	_ledkit.start(&led::animation::fire);
}

void ReinforcerKit::playSprinkles()
{
	_videokit.playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi",
							[this] { _ledkit.stop(); });
	_ledkit.start(&led::animation::sprinkles);
}

void ReinforcerKit::playRainbow()
{
	_videokit.playVideoOnce("/fs/home/vid/system/robot-system-reinforcer-happy-no_eyebrows.avi",
							[this] { _ledkit.stop(); });
	_ledkit.start(&led::animation::rainbow);
}
