// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>

#include "rtos/Kernel.h"

namespace leka::interface {

class LSM6DSOX
{
  public:
	virtual ~LSM6DSOX() = default;

	enum class PowerMode
	{
		Off,
		UltraLow,
		Normal,
		High,
	};

	// TODO(@ladislas) - user types --> move to include/types
	struct SensorData {
		using time_point_t = std::chrono::time_point<rtos::Kernel::Clock>;

		struct Accelerometer {
			float x = {};
			float y = {};
			float z = {};
		};

		struct Gyroscope {
			float x = {};
			float y = {};
			float z = {};
		};

		Accelerometer xl = {0, 0, 0};
		Gyroscope gy	 = {0, 0, 0};

		time_point_t timestamp {};
	};

	using drdy_callback_t = std::function<void(const SensorData)>;

	virtual void init()															= 0;
	virtual void registerOnGyDataReadyCallback(drdy_callback_t const &callback) = 0;
	virtual void setPowerMode(PowerMode)										= 0;
};
}	// namespace leka::interface
