// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <cstdint>
#include <span>

namespace leka {
enum class PowerMode : uint8_t
{
	low,
	normal
};

namespace interface {
	class TouchSensor
	{
	  public:
		virtual ~TouchSensor() = default;

		virtual auto read() -> int										= 0;
		virtual void setPowerMode(PowerMode power_mode)					= 0;
		virtual void setSensitivity(uint16_t value, bool saved = false) = 0;
	};

}	// namespace interface
}	// namespace leka
