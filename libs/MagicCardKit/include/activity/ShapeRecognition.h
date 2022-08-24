// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "CircularQueue.h"
#include "RFIDActivity.h"

namespace leka::rfid::activity {

class ShapeRecognition : public interface::RFIDActivity
{
  public:
	explicit ShapeRecognition() = default;

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

	static constexpr std::array<const char *, 8> shapes_table = {
		"shape-circle-plain_line",	  "shape-square-plain_line",  "shape-triangle-plain_line",
		"shape-star-plain_line",	  "shape-circle-dotted_line", "shape-square-dotted_line",
		"shape-triangle-dotted_line", "shape-star-dotted_line"};   // TODO (@hugo) Adapter avec les IDs

	CircularQueue<uint8_t, shapes_table.size() / 2> last_shapes_displayed {};

	MagicCard expected_tag_shape = MagicCard::none;
	uint8_t random_shape		 = 0;
	uint8_t pos					 = 0;

	std::string last_image = "NaN";

	void setRandomShapeDisplay();
};

}	// namespace leka::rfid::activity

// LCOV_EXCL_STOP
