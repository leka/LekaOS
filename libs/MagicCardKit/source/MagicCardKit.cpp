// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "MagicCardKit.h"

using namespace leka;

void MagicCardKit::init()
{
	_event_loop.registerCallback([this] { run(_magic_card); });
}

void MagicCardKit::start(interface::RFIDActivity &activity)
{
	stop();

	_activity = &activity;

	if (_activity == nullptr) {
		return;
	}

	_activity->setUtils(_reinforcerkit, _videokit, _led);
	_activity->start();
	_event_loop.start();
}

void MagicCardKit::updateCallback(const MagicCard &card)
{
	_event_loop.registerCallback([this] { run(_magic_card); });
}

void MagicCardKit::run(const MagicCard &card)
{
	_activity->run(card);
	if (_activity->playReinforcer()) {
		_reinforcerkit.runDefault();
	}
}

void MagicCardKit::stop()
{
	if (_activity != nullptr) {
		_activity->stop();
	}
}
