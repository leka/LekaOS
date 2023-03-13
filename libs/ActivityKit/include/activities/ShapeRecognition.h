// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include <optional>

#include "RFIDKit.h"
#include "ReinforcerKit.h"
#include "include/Shape.h"
#include "interface/Activity.h"
#include "interface/libs/VideoKit.h"

namespace leka::activity {

class ShapeRecognition : public interface::Activity
{
  public:
	explicit ShapeRecognition(RFIDKit &rfidkit, interface::VideoKit &videokit, ReinforcerKit &reinforcerkit)
		: _rfidkit(rfidkit), _videokit(videokit), _reinforcerkit(reinforcerkit) {};

	void start(const std::function<void()> &before_process_callback) final;
	void stop() final;

  private:
	void processCard(const MagicCard &card);
	void launchNextRound();

	RFIDKit &_rfidkit;
	interface::VideoKit &_videokit;
	ReinforcerKit &_reinforcerkit;

	static constexpr uint8_t kRoundsNumber	 = 8;
	static constexpr uint8_t kNumberOfShapes = 4 * 2;

	uint8_t _score = 0;
	std::optional<Shape> _current_shape {};
	std::function<void(const MagicCard &)> _backup_callback {};
	std::array<Shape, kNumberOfShapes> _shapes = {Shape::square,   Shape::square,	Shape::circle, Shape::circle,
												  Shape::triangle, Shape::triangle, Shape::star,   Shape::star};
};

}	// namespace leka::activity

// LCOV_EXCL_STOP
