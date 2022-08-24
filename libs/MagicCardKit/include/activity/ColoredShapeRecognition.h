// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "CircularQueue.h"
#include "RFIDActivity.h"

namespace leka::rfid::activity {

class ColoredShapeRecognition : public interface::RFIDActivity
{
  public:
	explicit ColoredShapeRecognition() = default;

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

	static constexpr std::array<const char *, 8> object_colored_family_table = {
		"animal-fish",		   "clothes-socks_pair", "clothes-tshirt",		  "object-balloon",
		"object-beach_bucket", "object-house",		 "plant-flower-5_petals", "vehicule-aerial-hot_air_balloon"};

	static constexpr std::array<const char *, 4> shapes_table = {"shape-circle-filled", "shape-square-filled",
																 "shape-triangle-filled", "shape-star-filled"};

	static constexpr auto number_of_different_colors = 4;
	CircularQueue<uint8_t, number_of_different_colors / 2> last_color_displayed {};
	CircularQueue<uint8_t, shapes_table.size() / 2> last_shape_displayed {};

	MagicCard expected_tag_color = MagicCard::none;
	MagicCard expected_tag_shape = MagicCard::none;
	MagicCard actual_tag_color	 = MagicCard::none;
	MagicCard actual_tag_shape	 = MagicCard::none;

	uint8_t random_color = 0;
	uint8_t random_shape = 0;
	uint8_t pos			 = 0;

	bool shape_already_load = false;
	bool color_already_load = false;

	std::string last_image = "NaN";

	void setRandomColoredShapeDisplay();
};

}	// namespace leka::rfid::activity

// LCOV_EXCL_STOP
