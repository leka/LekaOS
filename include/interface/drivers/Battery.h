// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <functional>

namespace leka::interface {

class Battery
{
  public:
	virtual ~Battery() = default;

	virtual void onChargeDidStart(std::function<void()> const &callback) = 0;
	virtual void onChargeDidStop(std::function<void()> const &callback)	 = 0;

	virtual auto voltage() -> float	  = 0;
	virtual auto level() -> uint8_t	  = 0;
	virtual auto isCharging() -> bool = 0;
};

}	// namespace leka::interface
