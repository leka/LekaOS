// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "CircularQueue.h"
#include "RFIDActivity.h"

namespace leka::rfid::activity {

class TwoNumbersAddition : public interface::RFIDActivity
{
  public:
	explicit TwoNumbersAddition() = default;

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

	static constexpr std::array<const char *, 11> numbers_table = {
		"number-0_zero", "number-1_one",   "number-2_two",	 "number-3_three", "number-4_four", "number-5_five",
		"number-6_six",	 "number-7_seven", "number-8_eight", "number-9_nine",  "number-10_ten"};

	CircularQueue<uint8_t, numbers_table.size() / 2> last_numbers_displayed {};

	MagicCard actual_tag_number_1 = MagicCard::none;
	MagicCard actual_tag_number_2 = MagicCard::none;

	uint8_t random_number = 0;
	uint8_t pos			  = 0;
	bool first_tag_valid  = false;

	std::string last_image = "NaN";

	void setRandomNumberDisplay();
};

}	// namespace leka::rfid::activity

// LCOV_EXCL_STOP
