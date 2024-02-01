// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace leka::interface {

class DeepSleepEnabled
{
  public:
	virtual ~DeepSleepEnabled() = default;

	virtual void enableDeepSleep()	= 0;
	virtual void disableDeepSleep() = 0;
};

}	// namespace leka::interface
