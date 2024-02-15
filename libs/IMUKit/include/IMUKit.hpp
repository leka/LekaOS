// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/IMU.hpp"
#include "interface/libs/IMUKit.hpp"

namespace leka {

class IMUKit : public interface::IMUKit
{
  public:
	explicit IMUKit(interface::IMU &imu) : _imu(imu) {}

	void init();
	void start() final;
	void stop() final;

	void setOrigin() final;
	void onEulerAnglesReady(angles_ready_callback_t const &callback) final;
	[[nodiscard]] auto getEulerAngles() const -> EulerAngles final;

  private:
	void drdy_callback(interface::IMU::SensorData data);

	interface::IMU &_imu;
	EulerAngles _euler_angles {};
	angles_ready_callback_t _on_euler_angles_rdy_callback {};
};

}	// namespace leka
