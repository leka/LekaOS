// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "MagicCard.h"
#include "RFIDActivity.h"
#include "SelectReinforcer.h"

namespace leka::interface {

class MagicCardKit
{
  public:
	virtual ~MagicCardKit() = default;

	virtual void init()								   = 0;
	virtual void start()							   = 0;
	virtual void run(const MagicCard &card)			   = 0;
	virtual void stop()								   = 0;
	virtual void updateCallback(const MagicCard &card) = 0;

	struct activity {
		static inline auto select_reinforcer = rfid::activity::SelectReinforcer {};
	};
};

}	// namespace leka::interface
