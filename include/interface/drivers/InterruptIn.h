// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>

#include "drivers/InterruptIn.h"

#include "Callback.h"
#include "PinNamesTypes.h"

namespace leka::interface {

class InterruptIn
{
  public:
	virtual ~InterruptIn() = default;

	virtual auto read() -> int = 0;

	virtual void rise(std::function<void()> const &callback) = 0;
	virtual void fall(std::function<void()> const &callback) = 0;
};

}	// namespace leka::interface
