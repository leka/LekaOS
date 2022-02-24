// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ColorKit.h"
#include "CoreLED.h"

namespace leka::interface {

class LEDAnimation
{
  public:
	virtual ~LEDAnimation() = default;

	virtual void start() = 0;
	virtual void stop()	 = 0;
	virtual void run()	 = 0;
};

}	// namespace leka::interface
