// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka::interface {

class TouchSensor
{
  public:
	virtual ~TouchSensor() = default;

	virtual void init()						 = 0;
	virtual auto read() -> bool				 = 0;
	virtual void reset()					 = 0;
	virtual void setSensitivity(float value) = 0;
};

}	// namespace leka::interface
