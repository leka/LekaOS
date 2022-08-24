// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "BehaviorKit.h"
#include "MagicCard.h"	 // TODO enlever

// TODO enum class reinforcer

namespace leka {

class ReinforcerKit
{
  public:
	explicit ReinforcerKit(BehaviorKit &behaviorkit) : _behaviorkit(behaviorkit) {};

	void runDefault();
	// void run(reinforcer);
	void setDefaultReinforcer(const MagicCard &card);	// TODO enum class reinforcer

  private:
	BehaviorKit &_behaviorkit;
	MagicCard _reinforcer = MagicCard::reinforcer_5_rainbow;
};

}	// namespace leka
