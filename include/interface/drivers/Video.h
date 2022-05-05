// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/platform/File.h"

namespace leka {

struct JPEGImageProperties;

}	// namespace leka

namespace leka::interface {

class Video
{
  public:
	virtual ~Video() = default;

	virtual void initialize() = 0;

	virtual void turnOff() = 0;
	virtual void turnOn()  = 0;

	virtual void setBrightness(float value) = 0;

	virtual void clearScreen()																		  = 0;
	virtual void displayImage(interface::File &file, JPEGImageProperties *image_properties = nullptr) = 0;

	virtual void setVideo(interface::File &file)			  = 0;
	virtual void displayNextFrameVideo(interface::File &file) = 0;
	virtual auto isLastFrame() -> bool						  = 0;
};

}	// namespace leka::interface
