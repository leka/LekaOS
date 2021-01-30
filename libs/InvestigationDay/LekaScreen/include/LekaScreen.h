// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_SCREEN_H_
#define _LEKA_OS_LIB_SCREEN_H_

#include "rtos/ThisThread.h"

#include "Display.h"
#include "FATFileSystem.h"
#include "SDBlockDevice.h"
#include "dma2d.h"
#include "otm8009a_conf.h"

class Screen
{
  public:
	Screen();
	~Screen() {};

	void start(void);

	int SDInit();

	void LTDCLayerInit();
	void clear(uint32_t ColorIndex);
	void drawRectangle(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t ColorIndex);

	void squareBouncing();
	void showFace(bool jpeg_file);

  private:
	SDBlockDevice _interface;
	FATFileSystem _file_interface;
	mbed::DigitalOut _sd_enable;
};

#endif
