// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "IMUKit.h"

using namespace leka;

void IMUKit::init()
{
	_mahony.begin(kDefaultSamplingConfig.frequency);

	auto on_drdy_callback = [this](const interface::LSM6DSOX::SensorData &imu_data) { computeAngles(imu_data); };
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
	_mahony.setOrigin();
}

auto IMUKit::getAngles() -> std::array<float, 3>
{
	return {_mahony.getPitch(), _mahony.getRoll(), _mahony.getYaw()};
}

void IMUKit::computeAngles(const interface::LSM6DSOX::SensorData &imu_data)
{
	auto xl_data			= std::make_tuple(imu_data.xl.x, imu_data.xl.y, imu_data.xl.z);
	auto gy_data			= std::make_tuple(imu_data.gy.x, imu_data.gy.y, imu_data.gy.z);
	constexpr auto mag_data = std::make_tuple(0.0F, 0.0F, 0.0F);
	_mahony.update(xl_data, gy_data, mag_data);
}
