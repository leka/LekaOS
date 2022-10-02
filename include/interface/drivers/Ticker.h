// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>
#include <functional>

namespace leka::interface {

class Ticker
{
  public:
	using callback_t = std::function<void()>;

	virtual ~Ticker() = default;

	virtual void onTick(callback_t const &callback) = 0;

	virtual void start(std::chrono::microseconds delay) = 0;
	virtual void stop()									= 0;
};

}	// namespace leka::interface
