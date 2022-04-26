// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "VideoKit.h"

#include "FileManagerKit.h"

using namespace leka;

void VideoKit::initializeScreen()
{
	_video.initialize();

	_video.setBrightness(1.F);
	_video.clearScreen();
}

void VideoKit::turnOn()
{
	_lcd.turnOn();
}

void VideoKit::turnOff()
{
	_lcd.turnOff();
}

void VideoKit::displayImage(const std::filesystem::path &path)
{
	auto file = FileManagerKit::File {path};

	if (file.is_open()) {
		_lcd.turnOn();

		_video.displayImage(file);

		file.close();
	}
}

void VideoKit::playVideo(const std::filesystem::path &path, bool must_loop)
{
	auto file = FileManagerKit::File {path};

	if (file.is_open()) {
		do {
			_lcd.turnOn();

			_video.playVideo(file);

		} while (must_loop);
		file.close();
	}
}

void VideoKit::stopVideo()
{
	// nothing to do
}
