// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Game.h"

namespace leka {

class GameKit
{
  public:
	explicit GameKit() = default;

	void start(interface::Game *game);
	void stop();

  private:
	void run();

	interface::Game *_game = nullptr;
};

}	// namespace leka
