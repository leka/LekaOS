// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>

#include "rtos/Kernel.h"

namespace leka::interface {

class IMU
{
  public:
	virtual ~IMU() = default;

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

	using data_ready_callback_t = std::function<void(const SensorData)>;

	virtual void init() = 0;

	virtual void registerOnDataReadyCallback(data_ready_callback_t const &callback) = 0;

	virtual void enableOnDataReadyInterrupt()  = 0;
	virtual void disableOnDataReadyInterrupt() = 0;

	virtual void registerOnWakeUpCallback(std::function<void()> const &callback) = 0;
	virtual void enableOnWakeUpInterrupt()										 = 0;
	virtual void disableOnWakeUpInterrupt()										 = 0;

	virtual void setPowerMode(PowerMode) = 0;
};
}	// namespace leka::interface
