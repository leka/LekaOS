// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "GameKit.h"

using namespace leka;

void GameKit::start(interface::Game *game)
{
	stop();

	_game = game;

	if (_game == nullptr) {
		return;
	}
	_game->start();
}

void GameKit::stop()
{
	if (_game != nullptr) {
		_game->stop();
	}
}
