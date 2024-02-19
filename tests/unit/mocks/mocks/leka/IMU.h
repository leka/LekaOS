// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/IMU.hpp"

namespace leka::mock {

class IMU : public interface::IMU
{
  public:
	MOCK_METHOD(void, init, (), (override));
	MOCK_METHOD(void, setPowerMode, (PowerMode), (override));

	void registerOnDataReadyCallback(drdy_callback_t const &cb) override { drdy_callback = cb; }

	void call_drdy_callback(const SensorData &data) { drdy_callback(data); }

  private:
	drdy_callback_t drdy_callback {};
};

}	// namespace leka::mock
