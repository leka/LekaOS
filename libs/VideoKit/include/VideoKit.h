// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/libs/VideoKit.h"

namespace leka {

class VideoKit : public interface::VideoKit
{
  public:
	explicit VideoKit() = default;

	void initializeScreen() final;

	void turnOn() final;
	void turnOff() final;

	void displayImage(const std::filesystem::path &path) final;

	void playVideo(const std::filesystem::path &path, bool must_loop = false) final;
	void stopVideo() final;
};

}	// namespace leka
