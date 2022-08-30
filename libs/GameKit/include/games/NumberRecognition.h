// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "Game.h"
#include "RFIDKit.h"
#include "ReinforcerKit.h"
#include "interface/libs/VideoKit.h"

namespace leka::game {

class NumberRecognition : public interface::Game
{
  public:
	explicit NumberRecognition(RFIDKit &rfidkit, interface::VideoKit &videokit, ReinforcerKit &reinforcerkit)
		: _rfidkit(rfidkit), _videokit(videokit), _reinforcerkit(reinforcerkit) {};

	void start() final;
	void stop() final;

  private:
	RFIDKit &_rfidkit;
	interface::VideoKit &_videokit;
	ReinforcerKit &_reinforcerkit;

	uint8_t pos					  = 0;
	uint8_t number_index		  = 0;
	MagicCard expected_tag_number = MagicCard::none;
	std::function<void(MagicCard &)> _backup_callback {};
	std::array<uint8_t, 11> index_table = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	std::span<uint8_t> index_span {index_table};

	static constexpr uint8_t kScoreToWin						= 5;
	static constexpr std::array<const char *, 11> numbers_table = {"000A", "000B", "000C", "000D", "000E", "000F",
																   "0010", "0011", "0012", "0013", "0014"};

	void setNextDisplay();
};

}	// namespace leka::game

// LCOV_EXCL_STOP
