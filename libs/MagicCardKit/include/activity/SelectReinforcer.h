// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "RFIDActivity.h"

namespace leka::rfid::activity {

class SelectReinforcer : public interface::RFIDActivity
{
  public:
	explicit SelectReinforcer() = default;

	void start() final;
	void run(const MagicCard &card) final;
	void stop() final;

	auto playReinforcer() -> bool final;
	void setUtils(ReinforcerKit &reinforcerkit, interface::VideoKit &videokit, interface::LED &led) final;
	void registerCallback(const std::function<void(const MagicCard &card)> &callback) final;

  private:
	bool _play_reinforcer = false;
	ReinforcerKit *_reinforcerkit {};
	interface::VideoKit *_videokit {};
	interface::LED *_led {};
	std::function<void(const MagicCard &card)> _set_reinforcer_callback {};
};

}	// namespace leka::rfid::activity

// LCOV_EXCL_STOP
