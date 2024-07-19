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

	void registerOnDataReadyCallback(data_ready_callback_t const &cb) override { data_ready_callback = cb; }
	MOCK_METHOD(void, enableOnDataReadyInterrupt, (), (override));
	MOCK_METHOD(void, disableOnDataReadyInterrupt, (), (override));

	void call_data_ready_callback(const SensorData &data) { data_ready_callback(data); }

  private:
	data_ready_callback_t data_ready_callback {};
};

}	// namespace leka::mock
