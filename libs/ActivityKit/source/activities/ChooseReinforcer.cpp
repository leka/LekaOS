// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "ChooseReinforcer.h"

namespace leka::activity {

void ChooseReinforcer::start()
{
	_videokit.displayImage("fs/home/img/system/robot-face-smiling-slightly.jpg");

	_backup_callback = _rfidkit.getCallback();

	_rfidkit.onTagActivated([this](const MagicCard &card) { processCard(card); });
}

void ChooseReinforcer::processCard(const MagicCard &card)
{
	switch (card.getId()) {
		case (MagicCard::reinforcer_1_blink_green.getId()):
			_reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::BlinkGreen);
			_reinforcerkit.playDefault();
			break;
		case (MagicCard::reinforcer_2_spin_blink.getId()):
			_reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::SpinBlink);
			_reinforcerkit.playDefault();
			break;
		case (MagicCard::reinforcer_3_fire.getId()):
			_reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::Fire);
			_reinforcerkit.playDefault();
			break;
		case (MagicCard::reinforcer_4_sprinkles.getId()):
			_reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::Sprinkles);
			_reinforcerkit.playDefault();
			break;
		case (MagicCard::reinforcer_5_rainbow.getId()):
			_reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::Rainbow);
			_reinforcerkit.playDefault();
			break;
		default:
			_backup_callback(card);
			break;
	}
}

void ChooseReinforcer::stop()
{
	_rfidkit.onTagActivated(_backup_callback);
}

}	// namespace leka::activity

// LCOV_EXCL_STOP
