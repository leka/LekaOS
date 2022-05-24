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

	MOCK_METHOD(void, playVideo, (const std::filesystem::path &, bool, std::function<void()>), (override));
	MOCK_METHOD(void, stopVideo, (), (override));
};

}	// namespace leka::mock
