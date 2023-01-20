// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ActivityKit.h"

using namespace leka;

void ActivityKit::registerActivities(std::unordered_map<MagicCard, interface::Activity *> const &activities)
{
	_activities = activities;
}

void ActivityKit::start(const MagicCard &card)
{
	stop();

	if (!_activities.contains(card)) {
		_current_activity = nullptr;
		return;
	}

	_current_activity = _activities.at(card);
	_current_activity->start();
}

void ActivityKit::stop()
{
	if (_current_activity == nullptr) {
		return;
	}

	_current_activity->stop();
	_current_activity = nullptr;
}

void ActivityKit::displayMainMenu(const MagicCard &card)
{
	if (card.getLanguage() == MagicCard::Language::fr_FR) {
		_videokit.displayImage("system/robot-misc-choose_activity-fr_FR.jpg");
	} else {
		_videokit.displayImage("system/robot-misc-choose_activity-en_US.jpg");
	}
}

auto ActivityKit::isPlaying() const -> bool
{
	return _current_activity != nullptr;
}
