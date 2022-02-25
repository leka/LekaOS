// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka::interface {

class CGAnimation
{
  public:
	virtual ~CGAnimation() = default;

	virtual void start() = 0;
	virtual void stop()	 = 0;
	virtual void run()	 = 0;
};

}	// namespace leka::interface
