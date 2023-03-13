// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "RFIDKit.h"
#include "ReinforcerKit.h"
#include "interface/Activity.h"
#include "interface/libs/VideoKit.h"

namespace leka::activity {

class FlashNumberCounting : public interface::Activity
{
  public:
	explicit FlashNumberCounting(RFIDKit &rfidkit, interface::VideoKit &videokit, ReinforcerKit &reinforcerkit,
								 interface::LED &led)
		: _rfidkit(rfidkit), _videokit(videokit), _reinforcerkit(reinforcerkit), _led(led) {};

	void start(const std::function<void()> &before_process_callback) final;
	void stop() final;

  private:
	void processCard(const MagicCard &card);
	void launchNextRound();

	RFIDKit &_rfidkit;
	interface::VideoKit &_videokit;
	ReinforcerKit &_reinforcerkit;
	interface::LED &_led;

	static constexpr uint8_t kRoundsNumber			 = 10;
	static constexpr uint8_t kSizeOfColorsTable		 = 5 * 2;
	static constexpr uint8_t kSizeOfFlashNumberTable = 12;

	static constexpr std::array<RGB, kSizeOfColorsTable> _colors = {
		RGB::pure_blue, RGB::pure_green, RGB::cyan, RGB::yellow, RGB::magenta,
		RGB::pure_blue, RGB::pure_green, RGB::cyan, RGB::yellow, RGB::magenta};

	uint8_t _current_round			= 0;
	uint8_t _current_flashes_number = 0;
	MagicCard _expected_tag_number	= MagicCard::none;
	std::function<void(const MagicCard &)> _backup_callback {};
	std::array<uint8_t, kSizeOfFlashNumberTable> _flash_numbers = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6};
};

}	// namespace leka::activity

// LCOV_EXCL_STOP
