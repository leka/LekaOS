// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <unordered_map>

#include "MagicCard.h"
#include "interface/Activity.h"

namespace leka {
class ActivityKit
{
  public:
	explicit ActivityKit() = default;

	void registerActivities(std::unordered_map<MagicCard, interface::Activity *> const &activities);

	void start(const MagicCard &card);
	void stop();

  private:
	interface::Activity *_current_activity = nullptr;
	std::unordered_map<MagicCard, interface::Activity *> _activities {};
};

}	// namespace leka
