// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/DeepSleepEnabled.h"

namespace leka::interface {

class LCDDriver : public interface::DeepSleepEnabled
{
  public:
	~LCDDriver() override = default;

	virtual void initialize()			   = 0;
	virtual void setLandscapeOrientation() = 0;

	virtual void turnOn()  = 0;
	virtual void turnOff() = 0;

	virtual void setBrightness(float value) = 0;
};

}	// namespace leka::interface
