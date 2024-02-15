// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/LSM6DSOX.hpp"
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
	void onEulerAnglesReady(angles_ready_callback_t const &callback) final;
	[[nodiscard]] auto getEulerAngles() const -> EulerAngles final;

  private:
	void drdy_callback(interface::LSM6DSOX::SensorData data);

	interface::LSM6DSOX &_lsm6dsox;
	EulerAngles _euler_angles {};
	angles_ready_callback_t _on_euler_angles_rdy_callback {};
};

}	// namespace leka
