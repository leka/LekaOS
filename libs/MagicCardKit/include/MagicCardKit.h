// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "RFIDActivity.h"
#include "ReinforcerKit.h"
#include "boost/sml.hpp"
#include "interface/libs/EventLoop.h"
#include "interface/libs/MagicCardKit.h"

namespace leka {

class MagicCardKit : public interface::MagicCardKit
{
  public:
	explicit MagicCardKit(interface::EventLoop &event_loop, ReinforcerKit &reinforcerkit, interface::VideoKit &videokit,
						  interface::LED &led)
		: _event_loop(event_loop), _reinforcerkit(reinforcerkit), _videokit(videokit), _led(led) {};

	void init() final;
	void start(interface::RFIDActivity &activity) final;
	void run(const MagicCard &card) final;
	void stop() final;
	void updateCallback(const MagicCard &card) final;

  private:
	interface::RFIDActivity *_activity = nullptr;
	interface::EventLoop &_event_loop;
	MagicCard _magic_card = MagicCard::none;
	ReinforcerKit &_reinforcerkit;
	interface::VideoKit &_videokit;
	interface::LED &_led;
};

}	// namespace leka
