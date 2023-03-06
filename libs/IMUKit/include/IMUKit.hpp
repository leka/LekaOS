// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/LSM6DSOX.hpp"
#include "interface/libs/IMUKit.hpp"

namespace leka {

class IMUKit : public interface::IMUKit
{
  public:
	explicit IMUKit(interface::LSM6DSOX &lsm6dsox) : _lsm6dsox(lsm6dsox) {}

	void init();
	void start() final;
	void stop() final;

	void setOrigin() final;
	[[nodiscard]] auto getEulerAngles() const -> EulerAngles final;

  private:
	void drdy_callback(interface::LSM6DSOX::SensorData data);

	interface::LSM6DSOX &_lsm6dsox;
	EulerAngles _euler_angles {};
};

}	// namespace leka
