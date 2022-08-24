// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ReinforcerKit.h"

using namespace leka;

void ReinforcerKit::setDefaultReinforcer(const MagicCard &card)
{
	_reinforcer = card;
}

void ReinforcerKit::runDefault()
{
	if (_reinforcer == MagicCard::reinforcer_1_blink_green) {
		_behaviorkit.blinkGreen();	 // TODO implémenter directement renforcateur avec leds, motors ...
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
