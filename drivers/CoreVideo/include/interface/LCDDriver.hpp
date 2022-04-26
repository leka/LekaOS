// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace leka::interface {

class LCDDriver
{
  public:
	virtual ~LCDDriver() = default;

	virtual void initialize()			   = 0;
	virtual void setLandscapeOrientation() = 0;

	virtual void turnOn()  = 0;
	virtual void turnOff() = 0;

	virtual void setBrightness(float value) = 0;
};

}	// namespace leka::interface
