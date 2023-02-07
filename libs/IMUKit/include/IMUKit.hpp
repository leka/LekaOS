// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/LSM6DSOX.hpp"

namespace leka {

struct EulerAngles {
	float pitch;
	float roll;
	float yaw;
};

class IMUKit
{
  public:
	explicit IMUKit(interface::LSM6DSOX &lsm6dsox) : _lsm6dsox(lsm6dsox) {}

	void init();
	void start();
	void stop();

	void setOrigin();
	auto getEulerAngles() -> const EulerAngles &;

  private:
	void drdy_callback(const interface::LSM6DSOX::SensorData &data);

	interface::LSM6DSOX &_lsm6dsox;
	EulerAngles _euler_angles {};
};

}	// namespace leka
