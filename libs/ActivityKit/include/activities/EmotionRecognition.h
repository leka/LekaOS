// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include <optional>

#include "RFIDKit.h"
#include "ReinforcerKit.h"
#include "include/Emotion.h"
#include "interface/Activity.h"
#include "interface/libs/VideoKit.h"

namespace leka::activity {

class EmotionRecognition : public interface::Activity
{
  public:
	explicit EmotionRecognition(RFIDKit &rfidkit, interface::VideoKit &videokit, ReinforcerKit &reinforcerkit)
		: _rfidkit(rfidkit), _videokit(videokit), _reinforcerkit(reinforcerkit) {};

	void start() final;
	void stop() final;

  private:
	void processCard(const MagicCard &card);
	void launchNextRound();

	RFIDKit &_rfidkit;
	interface::VideoKit &_videokit;
	ReinforcerKit &_reinforcerkit;

	static constexpr uint8_t kRoundsNumber		  = 10;
	static constexpr uint8_t kSizeOfEmotionsTable = 5 * 2;

	uint8_t _current_round = 0;
	std::optional<Emotion> _current_emotion {};
	std::function<void(const MagicCard &)> _backup_callback {};
	std::array<Emotion, kSizeOfEmotionsTable> _emotions = {
		Emotion::anger, Emotion::anger,	  Emotion::fear,	Emotion::fear,	  Emotion::joy,
		Emotion::joy,	Emotion::sadness, Emotion::sadness, Emotion::disgust, Emotion::disgust};
};

}	// namespace leka::activity

// LCOV_EXCL_STOP
