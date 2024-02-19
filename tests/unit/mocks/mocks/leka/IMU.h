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
	MOCK_METHOD(void, enableOnDataAvailable, (), (override));
	MOCK_METHOD(void, disableOnDataAvailable, (), (override));

	void registerOnWakeUpCallback(std::function<void()> const &cb) override { wake_up_callback = cb; }
	MOCK_METHOD(void, enableOnWakeUpInterrupt, (), (override));
	MOCK_METHOD(void, disableOnWakeUpInterrupt, (), (override));

	void call_data_available_callback(const SensorData &data) { data_available_callback(data); }
	void call_wake_up_callback() { wake_up_callback(); }

  private:
	data_available_callback_t data_available_callback {};
	std::function<void()> wake_up_callback {};
};

}	// namespace leka::mock
