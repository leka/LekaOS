// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Activity.h"

namespace leka {

class ActivityKit
{
  public:
	explicit ActivityKit() = default;

	void start(interface::Activity *activity);
	void stop();

  private:
	interface::Activity *_activity = nullptr;
};

}	// namespace leka
