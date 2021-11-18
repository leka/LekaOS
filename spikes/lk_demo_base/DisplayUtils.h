// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DISPLAY_UTILS_H_
#define _LEKA_OS_DISPLAY_UTILS_H_

#include "rtos/EventFlags.h"
#include "rtos/Thread.h"

#include "CoreSDRAM.hpp"
#include "FATFileSystem.h"
#include "Flags.h"
#include "LKCoreFatFs.h"
#include "LKCoreSTM32Hal.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

namespace leka {

class DisplayUtils
{
  public:
	explicit DisplayUtils(rtos::Thread &video_thread, rtos::EventFlags &event_flags, LKCoreSTM32Hal &hal,
						  CoreSDRAM &sdram, VideoKit &display)
		: _video_thread(video_thread), _event_flags(event_flags), _hal(hal), _coresdram(sdram), _display(display) {};

	void initializeSD();
	void initializeScreen();

	void setOn();
	void setOff();
	void setBrightness(float value);

	auto fileExists(lstd::span<const char> full_path);

	void displayImage(std::string filename);
	void displayVideo(std::string filename);

  private:
	void runVideo();

	rtos::Thread &_video_thread;
	rtos::EventFlags &_event_flags;

	SDBlockDevice _sd_blockdevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
	FATFileSystem _fatfs {"fs"};

	float _brightness_on_default {1.F};

	LKCoreSTM32Hal &_hal;

	CoreSDRAM &_coresdram;
	VideoKit &_display;

	std::string _full_path_video {};
};

}	// namespace leka

#endif	 // _LEKA_OS_DISPLAY_UTILS_H_
