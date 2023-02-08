// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/LSM6DSOX.hpp"

namespace leka::mock {

class LSM6DSOX : public interface::LSM6DSOX
{
  public:
	MOCK_METHOD(void, init, (), (override));
	MOCK_METHOD(void, setPowerMode, (PowerMode), (override));

	void registerOnGyDataReadyCallback(std::function<void(const SensorData &)> const &cb) override
	{
		drdy_callback = cb;
	}

	void call_drdy_callback(const SensorData &data) { drdy_callback(data); }

  private:
	std::function<void(const SensorData &)> drdy_callback {};
};

}	// namespace leka::mock
