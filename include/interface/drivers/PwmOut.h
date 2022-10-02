// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace leka::interface {

class PwmOut
{
  public:
	virtual ~PwmOut() = default;

	virtual auto read() -> float	= 0;
	virtual void write(float value) = 0;
};

}	// namespace leka::interface
