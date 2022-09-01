// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <unordered_map>

#include "Activity.h"
#include "MagicCard.h"
#include "interface/libs/Activity.h"
namespace leka {

class ActivityKit
{
  public:
	explicit ActivityKit() = default;

	void registerActivities(std::unordered_map<uint16_t, interface::Activity *> const &activities);
	void start(const MagicCard &card);
	void stop();

  private:
	interface::Activity *_activity = nullptr;
	std::unordered_map<uint16_t, interface::Activity *> _activities {};
};

}	// namespace leka
