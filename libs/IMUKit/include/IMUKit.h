// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>

#include "interface/LSM6DSOX.h"
#include "internal/Mahony.h"

namespace leka {

class IMUKit
{
  public:
	explicit IMUKit(interface::LSM6DSOX &lsm6dsox)
		: _lsm6dsox(lsm6dsox) {
			  // nothing to do
		  };

	void init();
	void start();
	void stop();
	void setOrigin();

	auto getAngles() -> std::array<float, 3>;

  private:
	void computeAngles(const interface::LSM6DSOX::SensorData &imu_data);
	interface::LSM6DSOX &_lsm6dsox;

	ahrs::Mahony _mahony {};
	struct SamplingConfig {
		const std::chrono::milliseconds delay {};
		const float frequency {};
	};
	// ? Sampling config deprecated.
	// TODO(@ladislas @hugo): Use dynamic sampling frequency.
	const SamplingConfig kDefaultSamplingConfig {.delay = std::chrono::milliseconds {70}, .frequency = 13.F};
};

}	// namespace leka
