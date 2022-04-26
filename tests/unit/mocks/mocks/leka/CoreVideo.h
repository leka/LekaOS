// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/Video.h"

namespace leka::mock {

class CoreVideo : public interface::Video
{
  public:
	MOCK_METHOD(void, initialize, (), (override));

	MOCK_METHOD(void, turnOn, (), (override));
	MOCK_METHOD(void, turnOff, (), (override));

	MOCK_METHOD(void, setBrightness, (float value), (override));

	MOCK_METHOD(void, clearScreen, (), (override));
	MOCK_METHOD(void, displayImage, (interface::File & file), (override));
	MOCK_METHOD(void, playVideo, (interface::File & file), (override));
};

}	// namespace leka::mock