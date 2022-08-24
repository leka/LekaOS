// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "CircularQueue.h"
#include "RFIDActivity.h"

namespace leka::rfid::activity {

// TODO (@hugo) Couleurs sur les Leds ou bien couleurs des objets existants

class ColorRecognition : public interface::RFIDActivity
{
  public:
	explicit ColorRecognition() = default;

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

	static constexpr auto number_of_different_colors = 6;	// TODO rename
	CircularQueue<uint8_t, number_of_different_colors / 2> last_color_displayed {};
	CircularQueue<uint8_t, object_colored_family_table.size() / 2> last_family_displayed {};

	MagicCard expected_tag_color = MagicCard::none;
	uint8_t random_color		 = 0;
	uint8_t random_family		 = 0;
	uint8_t pos					 = 0;

	std::string last_image = "NaN";

	void setRandomColorAndFamilyDisplay();
};

}	// namespace leka::rfid::activity

// LCOV_EXCL_STOP
