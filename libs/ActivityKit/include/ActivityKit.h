// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <interface/libs/VideoKit.h>
#include <unordered_map>

#include "MagicCard.h"
#include "interface/Activity.h"

namespace leka {
class ActivityKit
{
  public:
	explicit ActivityKit(interface::VideoKit &videokit) : _videokit(videokit) {};

	void registerActivities(std::unordered_map<MagicCard, interface::Activity *> const &activities);
	void registerBeforeProcessCallback(std::function<void()> callback);

	void start(const MagicCard &card);
	void stop();

	void displayMainMenu(const MagicCard &card);

	[[nodiscard]] auto isPlaying() const -> bool;

  private:
	std::function<void()> _before_process_callback {};

	interface::VideoKit &_videokit;
	interface::Activity *_current_activity = nullptr;
	std::unordered_map<MagicCard, interface::Activity *> _activities {};
};

}	// namespace leka
