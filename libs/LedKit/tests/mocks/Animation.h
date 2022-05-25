// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "LEDAnimation.h"
#include "gmock/gmock.h"

namespace leka::mock {

class Animation : public interface::LEDAnimation
{
  public:
	explicit Animation() = default;

	void start() final
	{
		startCalled();
		_running = true;
		_stage	 = 0;
	}
	void run() final
	{
		auto kMaxStageNumber = 10;
		if (_stage < kMaxStageNumber) {
			++_stage;
			stageCalled();
		} else {
			_running = false;
		}
	}
	void stop() final
	{
		stopCalled();
		_running = false;
	}
	auto isRunning() -> bool final { return _running; }

	MOCK_METHOD(void, setLeds, (interface::LED &, interface::LED &), (override));
	MOCK_METHOD(void, startCalled, (), ());
	MOCK_METHOD(void, stopCalled, (), ());
	MOCK_METHOD(void, stageCalled, (), ());

  private:
	bool _running  = false;
	uint8_t _stage = 0;
};

}	// namespace leka::mock
