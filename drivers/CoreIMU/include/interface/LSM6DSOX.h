// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

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
	};

	virtual void init()					   = 0;
	virtual auto getData() -> SensorData & = 0;
	virtual void setPowerMode(PowerMode)   = 0;
};
}	// namespace leka::interface
