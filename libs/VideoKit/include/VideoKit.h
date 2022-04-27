// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/LCD.hpp"
#include "interface/drivers/Video.h"
#include "interface/libs/VideoKit.h"

namespace leka {

class VideoKit : public interface::VideoKit
{
  public:
	explicit VideoKit(interface::LCD &lcd, interface::Video &video) : _lcd(lcd), _video {video}
	{
		// nothing to do
	}

	void initializeScreen() final;

	void displayImage(const std::filesystem::path &path) final;

	void playVideo(const std::filesystem::path &path, bool must_loop = false) final;
	void stopVideo() final;

  private:
	interface::LCD &_lcd;
	interface::Video &_video;
};

}	// namespace leka
