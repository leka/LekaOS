// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <functional>

namespace leka::interface {

class BatteryKit
{
  public:
	virtual ~BatteryKit() = default;

	virtual void startEventHandler() = 0;

	virtual auto level() -> uint8_t											 = 0;
	virtual void onDataUpdated(std::function<void(uint8_t)> const &callback) = 0;
	virtual void onLowBattery(std::function<void()> const &callback)		 = 0;

	virtual auto isCharging() -> bool									 = 0;
	virtual void onChargeDidStart(std::function<void()> const &callback) = 0;
	virtual void onChargeDidStop(std::function<void()> const &callback)	 = 0;
};

}	// namespace leka::interface
