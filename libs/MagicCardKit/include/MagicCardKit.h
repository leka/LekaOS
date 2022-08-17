// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/HighResClock.h"

#include "RFIDActivity.h"
#include "StateMachineActivity.h"
#include "boost/sml.hpp"
#include "interface/libs/EventLoop.h"
#include "interface/libs/MagicCardKit.h"

namespace leka {

class MagicCardKit : public interface::MagicCardKit
{
  public:
	explicit MagicCardKit(interface::EventLoop &event_loop, BehaviorKit &behaviorkit)
		: _event_loop(event_loop), _behaviorkit(behaviorkit) {};

	void init() final;
	void start() final;
	void run(const MagicCard &card) final;
	void stop() final;
	void updateCallback(const MagicCard &card) final;

  private:
	interface::RFIDActivity *_activity = nullptr;
	interface::EventLoop &_event_loop;
	MagicCard _magic_card = MagicCard::none;
	BehaviorKit &_behaviorkit;
	MagicCard _reinforcer = MagicCard::reinforcer_5_rainbow;

	void linkMagicCardToActivity(const MagicCard &card);
	void runReinforcer();
	void setDefaultReinforcer(const MagicCard &card);
};

}	// namespace leka
