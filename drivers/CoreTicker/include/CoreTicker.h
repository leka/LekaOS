// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/LowPowerTicker.h"

#include "interface/drivers/Ticker.h"

namespace leka {

class CoreTicker : public interface::Ticker
{
  public:
	explicit CoreTicker() = default;

	void onTick(callback_t const &callback) final;

	void start(std::chrono::microseconds delay) final;
	void stop() final;

  private:
	void tick() const;

	callback_t _callback {};

	mbed::LowPowerTicker _ticker {};
};

}	// namespace leka
