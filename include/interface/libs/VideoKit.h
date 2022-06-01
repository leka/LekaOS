// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>

namespace leka::interface {

class VideoKit
{
  public:
	virtual ~VideoKit() = default;

	virtual void initializeScreen() = 0;

	virtual void displayImage(const std::filesystem::path &path) = 0;

	virtual void playVideoOnce(const std::filesystem::path &path)	  = 0;
	virtual void playVideoOnRepeat(const std::filesystem::path &path) = 0;

	virtual void stopVideo() = 0;
};

}	// namespace leka::interface
