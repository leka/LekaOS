// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <interface/libs/EventLoop.h>

#include "Game.h"

namespace leka {

class GameKit
{
  public:
	explicit GameKit(interface::EventLoop &event_loop) : _event_loop(event_loop) {}

	void init();
	void start(interface::Game *game);
	void stop();

  private:
	void run();

	interface::EventLoop &_event_loop;
	interface::Game *_game = nullptr;
};

}	// namespace leka
