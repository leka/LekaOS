// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "IMUKit.h"

#include "rtos/ThisThread.h"

using namespace leka;

void IMUKit::init()
{
	_mahony.begin(kDefaultSamplingConfig.frequency);
	_event_loop.registerCallback([this] { run(); });
}

void IMUKit::start()
{
	_is_running = true;
	_event_loop.start();
}

void IMUKit::run()
{
	while (_is_running) {
		computeAngles();
		rtos::ThisThread::sleep_for(kDefaultSamplingConfig.delay);
	}
}

void IMUKit::stop()
{
	_is_running = false;
	_event_loop.stop();
}

auto IMUKit::getAngles() -> std::array<float, 3>
{
	return {_mahony.getPitch(), _mahony.getRoll(), _mahony.getYaw()};
}

void IMUKit::reset()
{
	_mahony.setOrigin();
}

void IMUKit::computeAngles()
{
	auto accel = _accel.getXYZ();
	auto gyro  = _gyro.getXYZ();
	auto mag   = std::make_tuple(0.0F, 0.0F, 0.0F);
	_mahony.update(accel, gyro, mag);
}
