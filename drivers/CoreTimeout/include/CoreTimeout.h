// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/LowPowerTimeout.h"

#include "interface/drivers/Timeout.h"

namespace leka {

class CoreTimeout : public interface::Timeout
{
  public:
	explicit CoreTimeout() = default;

	void onTimeout(callback_t const &callback) final;

	void start(std::chrono::microseconds countdown) final;
	void stop() final;

  private:
	void timeout() const;

	callback_t _callback {};

	mbed::LowPowerTimeout _timeout {};
};

}	// namespace leka
