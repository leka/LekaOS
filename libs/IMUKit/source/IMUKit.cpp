// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "IMUKit.hpp"

using namespace leka;

void IMUKit::init()
{
	auto on_drdy_callback = [this](const interface::LSM6DSOX::SensorData &data) {
		// TODO(@ladislas): to implement
	};

	_lsm6dsox.registerOnGyDataReadyCallback(on_drdy_callback);
}

void IMUKit::start()
{
	_lsm6dsox.setPowerMode(interface::LSM6DSOX::PowerMode::Normal);
}

void IMUKit::stop()
{
	_lsm6dsox.setPowerMode(interface::LSM6DSOX::PowerMode::Off);
}

void IMUKit::setOrigin()
{
	// TODO(@ladislas): to implement
}

auto IMUKit::getEulerAngles() -> const EulerAngles &
{
	return _euler_angles;
}
