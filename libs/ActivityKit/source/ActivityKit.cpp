// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ActivityKit.h"

using namespace leka;

void ActivityKit::registerActivities(std::unordered_map<uint16_t, interface::Activity *> const &activities)
{
	_activities = activities;
}

void ActivityKit::start(const MagicCard &card)
{
	stop();

	if (_activities.find(card.getId()) == _activities.end()) {
		_current_activity = nullptr;
		return;
	}

	_current_activity = _activities.at(card.getId());

	_current_activity->start();
}

void ActivityKit::stop()
{
	if (_current_activity != nullptr) {
		_current_activity->stop();
	}
}
