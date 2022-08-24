// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "CircularQueue.h"
#include "RFIDActivity.h"

namespace leka::rfid::activity {

class EmotionRecognition : public interface::RFIDActivity
{
  public:
	explicit EmotionRecognition() = default;

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

	static constexpr std::array<const char *, 5> emotions_table = {"robot-emotion-happy", "robot-emotion-angry",
																   "robot-emotion-affraid", "robot-emotion-disgusted",
																   "robot-emotion-sad_tears"};

	CircularQueue<uint8_t, emotions_table.size() / 2> last_emotions_displayed {};

	MagicCard expected_tag_emotion_child = MagicCard::none;
	MagicCard expected_tag_emotion_leka	 = MagicCard::none;
	uint8_t random_emotion				 = 0;
	uint8_t pos							 = 0;

	std::string last_image = "NaN";

	void setRandomEmotionDisplay();
};

}	// namespace leka::rfid::activity

// LCOV_EXCL_STOP
