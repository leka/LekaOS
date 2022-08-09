// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "GameKit.h"

#include "rtos/ThisThread.h"

#include "Game.h"

using namespace leka;
using namespace std::chrono;

void GameKit::init()
{
	_event_loop.registerCallback([this] { run(); });
}

void GameKit::start(interface::Game *game)
{
	stop();

	_game = game;

	if (_game == nullptr) {
		return;
	}
	_game->start();
	_event_loop.start();
}

void GameKit::run()
{
	while (_game->isRunning()) {
		_game->run();
		rtos::ThisThread::sleep_for(50ms);
	}
}

void GameKit::stop()
{
	_event_loop.stop();

	if (_game != nullptr) {
		_game->stop();
	}
}
