// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/platform/File.h"

namespace leka::interface {

class Video
{
  public:
	virtual ~Video() = default;

	virtual void initialize() = 0;

	virtual void turnOff() = 0;
	virtual void turnOn()  = 0;

	virtual void setBrightness(float value) = 0;

	virtual void clearScreen()						 = 0;
	virtual void displayImage(interface::File &file) = 0;
	virtual void playVideo(interface::File &file)	 = 0;
};

}	// namespace leka::interface
