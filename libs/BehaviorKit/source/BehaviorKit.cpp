// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BehaviorKit.h"

#include "rtos/ThisThread.h"

namespace leka {

using namespace std::chrono;

void BehaviorKit::spinLeft(float speed)
{
	_motor_left.spin(Rotation::clockwise, speed);
	_motor_right.spin(Rotation::clockwise, speed);
}

void BehaviorKit::spinRight(float speed)
{
	_motor_left.spin(Rotation::counterClockwise, speed);
	_motor_right.spin(Rotation::counterClockwise, speed);
}

void BehaviorKit::onScreensaverEnded(std::function<void(void)> const &callback)
{
	_on_screensaver_ended = callback;
}

void BehaviorKit::launching()
{
	_videokit.displayImage("/fs/images/logo.jpg");
}

void BehaviorKit::wakingUp()
{
	_ledkit.start(&LedKit::animation::wake_up);
	_videokit.playVideo("/fs/videos/2022_02_14-animation-face-action-waking-up-without-eyebrows.avi", false,
						[this] { neutral(); });
}

void BehaviorKit::sleeping()
{
	_ledkit.start(&LedKit::animation::sleeping);
	_videokit.playVideo("/fs/videos/2022_01_17-animation-face-state-yawning-sleeping_without_eyebrows.avi");
}

void BehaviorKit::waiting()
{
	_ledkit.stop();
	_videokit.playVideo(
		"/fs/videos/2022_02_14-animation-face-state-waiting-looking-top-right-to-left-without-eyebrows.avi", true);
}

void BehaviorKit::lowBattery()
{
	_ledkit.stop();
	_videokit.displayImage("/fs/images/loading.jpg");
}

void BehaviorKit::chargingZero()
{
	_videokit.displayImage("/fs/images/low_battery.jpg");
}

void BehaviorKit::chargingRed()
{
	_videokit.displayImage("/fs/images/battery_red.jpg");
}

void BehaviorKit::chargingOrange()
{
	_videokit.displayImage("/fs/images/battery_yellow_2.jpg");
}

void BehaviorKit::chargingYellow()
{
	_videokit.displayImage("/fs/images/battery_green_3.jpg");
}

void BehaviorKit::chargingGreen()
{
	_videokit.displayImage("/fs/images/battery_green_4.jpg");
}

void BehaviorKit::underwater()
{
	_ledkit.start(&LedKit::animation::underwater);
	_videokit.playVideo("/fs/videos/2022_02_14-animation-face-state-under-water-without-eyebrows.avi", false,
						_on_screensaver_ended);
}

void BehaviorKit::bubbles()
{
	_ledkit.start(&LedKit::animation::bubbles);
	_videokit.playVideo("/fs/videos/2022_02_14-animation-face-action-bubbles-without-eyebrows.avi", false,
						_on_screensaver_ended);
}

void BehaviorKit::fly()
{
	_ledkit.start(&LedKit::animation::fly);
	_videokit.playVideo("/fs/videos/2022_02_14-animation-face-action-fly-without-eyebrows.avi", false,
						_on_screensaver_ended);
}

void BehaviorKit::singing()
{
	_ledkit.start(&LedKit::animation::singing);
	_videokit.playVideo("/fs/videos/2022_02_14-animation-face-action-singing-color-without-eyebrows.avi", false,
						_on_screensaver_ended);
}

void BehaviorKit::blinkGreen()
{
	spinLeft(1);
	_ledkit.start(&LedKit::animation::blink_green);
	_videokit.playVideo("/fs/videos/2022_02_14-animation-face-state-happy-without-eyebrows.avi");
	rtos::ThisThread::sleep_for(3s);
	_ledkit.stop();
	_motor_left.stop();
	_motor_right.stop();
}

void BehaviorKit::spinBlink()
{
	spinRight(1);
	_ledkit.start(&LedKit::animation::spin_blink);
	_videokit.playVideo("/fs/videos/2022_02_14-animation-face-state-happy-without-eyebrows.avi");
	rtos::ThisThread::sleep_for(3s);
	_ledkit.stop();
	_motor_left.stop();
	_motor_right.stop();
}

void BehaviorKit::fire()
{
	_ledkit.start(&LedKit::animation::fire);
	_videokit.playVideo("/fs/videos/2022_02_14-animation-face-state-happy-without-eyebrows.avi");
	rtos::ThisThread::sleep_for(3s);
	_ledkit.stop();
}

void BehaviorKit::sprinkles()
{
	_ledkit.start(&LedKit::animation::sprinkles);
	_videokit.playVideo("/fs/videos/2022_02_14-animation-face-state-happy-without-eyebrows.avi");
	rtos::ThisThread::sleep_for(3s);
	_ledkit.stop();
}

void BehaviorKit::rainbow()
{
	_ledkit.start(&LedKit::animation::rainbow);
	_videokit.playVideo("/fs/videos/2022_02_14-animation-face-state-happy-without-eyebrows.avi");
	rtos::ThisThread::sleep_for(3s);
	_ledkit.stop();
}

void BehaviorKit::bleConnection(bool enableVideo)
{
	_ledkit.start(&LedKit::animation::ble_connection);
	if (enableVideo) {
		_videokit.playVideo("/fs/videos/2022_02_14-animation-face-action-wink-without-eyebrows.avi");
	}
}

void BehaviorKit::neutral()
{
	_videokit.displayImage("/fs/images/robot-emotion-neutral.jpg");
}

void BehaviorKit::stop()
{
	_ledkit.stop();
	_videokit.stopVideo();
	_motor_left.stop();
	_motor_right.stop();
}

}	// namespace leka
