// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace leka {

class VideoKit
{
  public:
	explicit VideoKit() = default;

	void initializeScreen() {};

	void turnOn() {};
	void turnOff() {};

	void displayImage(const char *path) {};

	void playVideo(const char *path) {};
	void stopVideo() {};
};

}	// namespace leka
