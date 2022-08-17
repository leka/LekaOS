// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "MagicCardKit.h"

using namespace leka;

void MagicCardKit::init()
{
	_event_loop.registerCallback([this] { run(_magic_card); });
}

void MagicCardKit::start()
{
	stop();

	linkMagicCardToActivity(_magic_card);

	if (_activity == nullptr) {
		return;
	}

	_activity->setBehaviorKit(_behaviorkit);
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
		runReinforcer();
	}
}

void MagicCardKit::stop()
{
	if (_activity != nullptr) {
		_activity->stop();
	}
}

void MagicCardKit::linkMagicCardToActivity(const MagicCard &card)
{
	switch (card.getId()) {
		case 10:
			_activity							 = &interface::MagicCardKit::activity::select_reinforcer;
			auto set_default_reinforcer_callback = [this](const MagicCard &card) { setDefaultReinforcer(card); };
			_activity->registerCallback(set_default_reinforcer_callback);
			break;
	}
}

void MagicCardKit::setDefaultReinforcer(const MagicCard &card)
{
	_reinforcer = card;
}

void MagicCardKit::runReinforcer()
{
	if (_reinforcer == MagicCard::reinforcer_1_blink_green) {
		_behaviorkit.blinkGreen();
	} else if (_reinforcer == MagicCard::reinforcer_2_spin_blink) {
		_behaviorkit.spinBlink();
	} else if (_reinforcer == MagicCard::reinforcer_3_fire) {
		_behaviorkit.fire();
	} else if (_reinforcer == MagicCard::reinforcer_4_sprinkles) {
		_behaviorkit.sprinkles();
	} else {
		_behaviorkit.rainbow();
	}
}
