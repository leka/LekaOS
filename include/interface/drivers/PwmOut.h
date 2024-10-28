// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "DeepSleepEnabled.h"

namespace leka::interface {

class PwmOut : public interface::DeepSleepEnabled
{
  public:
	virtual ~PwmOut() = default;

	virtual void period(float value) = 0;

	virtual auto read() -> float	= 0;
	virtual void write(float value) = 0;
};

}	// namespace leka::interface
