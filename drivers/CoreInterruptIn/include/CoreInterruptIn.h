// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/InterruptIn.h"

namespace leka {

class CoreInterruptIn : public interface::InterruptIn
{
  public:
	explicit CoreInterruptIn(PinName pin) : _interrupt_in(pin) { _event_queue.dispatch_forever(); };

	auto read() -> int final;

	void rise(std::function<void()> const &callback) final;
	void fall(std::function<void()> const &callback) final;

  private:
	PinName _pin {NC};
	mbed::InterruptIn _interrupt_in {_pin};
	std::function<void()> _on_rise_callback {};
	std::function<void()> _on_fall_callback {};
};

}	// namespace leka
