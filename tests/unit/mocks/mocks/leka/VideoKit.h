// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/libs/VideoKit.h"

namespace leka::mock {

class VideoKit : public interface::VideoKit
{
  public:
	MOCK_METHOD(void, initializeScreen, (), (override));

	MOCK_METHOD(void, displayImage, (const std::filesystem::path &), (override));
	MOCK_METHOD(void, fillWhiteBackgroundAndDisplayImage, (const std::filesystem::path &), (override));

	MOCK_METHOD(void, playVideoOnce, (const std::filesystem::path &, const std::function<void()> &), (override));
	MOCK_METHOD(void, playVideoOnRepeat, (const std::filesystem::path &, const std::function<void()> &), (override));

	MOCK_METHOD(void, stopVideo, (), (override));
};

}	// namespace leka::mock
