// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "BehaviorKit.h"
#include "MagicCard.h"

namespace leka::interface {

class RFIDActivity
{
  public:
	virtual ~RFIDActivity() = default;

	virtual void start()					= 0;
	virtual void stop()						= 0;
	virtual void run(const MagicCard &card) = 0;

	virtual auto playReinforcer() -> bool													  = 0;
	virtual void setBehaviorKit(BehaviorKit &behaviorkit)									  = 0;
	virtual void registerCallback(const std::function<void(const MagicCard &card)> &callback) = 0;
};

}	// namespace leka::interface
