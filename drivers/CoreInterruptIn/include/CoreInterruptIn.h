// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/InterruptIn.h"

namespace leka {

class CoreInterruptIn : public interface::InterruptIn
{
  public:
	explicit CoreInterruptIn(PinName pin) : _irq(pin) {};

	auto read() -> int final;

	void onRise(std::function<void()> const &callback) final;
	void onFall(std::function<void()> const &callback) final;

  private:
	mbed::InterruptIn _irq;
	std::function<void()> _on_rise_callback {};
	std::function<void()> _on_fall_callback {};
};

}	// namespace leka
