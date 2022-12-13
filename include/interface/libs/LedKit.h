// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "LEDAnimation.h"

namespace leka::interface {

class LedKit
{
  public:
	virtual ~LedKit() = default;

	virtual void init()									   = 0;
	virtual void start(interface::LEDAnimation *animation) = 0;
	virtual void run()									   = 0;
	virtual void stop()									   = 0;
};

}	// namespace leka::interface
