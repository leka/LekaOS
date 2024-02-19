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

	void registerOnDataAvailableCallback(data_available_callback_t const &cb) override { data_available_callback = cb; }

	void call_data_available_callback(const SensorData &data) { data_available_callback(data); }

  private:
	data_available_callback_t data_available_callback {};
};

}	// namespace leka::mock
