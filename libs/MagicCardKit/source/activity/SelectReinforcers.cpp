// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "SelectReinforcer.h"

namespace leka::rfid::activity {

auto SelectReinforcer::playReinforcer() -> bool
{
	return _play_reinforcer;
}

void SelectReinforcer::setBehaviorKit(BehaviorKit &behaviorkit)
{
	_behaviorkit = &behaviorkit;
}

void SelectReinforcer::registerCallback(const std::function<void(const MagicCard &card)> &callback)
{
	_set_reinforcer_callback = callback;
}

void SelectReinforcer::start()
{
	_behaviorkit->working();
	_play_reinforcer = false;
}

void SelectReinforcer::stop()
{
	_behaviorkit->stop();
	_play_reinforcer = false;
}

void SelectReinforcer::run(const MagicCard &card)
{
	auto is_card_a_reinforcer = card == MagicCard::reinforcer_1_blink_green ||
								card == MagicCard::reinforcer_2_spin_blink || card == MagicCard::reinforcer_3_fire ||
								card == MagicCard::reinforcer_4_sprinkles || card == MagicCard::reinforcer_5_rainbow;
	if (is_card_a_reinforcer) {
		_set_reinforcer_callback(card);
		_play_reinforcer = true;
	}
}

}	// namespace leka::rfid::activity

// LCOV_EXCL_STOP
