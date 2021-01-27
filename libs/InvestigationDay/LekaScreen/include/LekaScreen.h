// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_SCREEN_H_
#define _LEKA_OS_LIB_SCREEN_H_

#include "rtos/ThisThread.h"

#include "Display.h"
#include "FATFileSystem.h"
#include "SDBlockDevice.h"
#include "otm8009a_conf.h"

class Screen
{
  public:
	Screen();
	~Screen() {};

	void start(void);

	int SDInit();
	void getFileSize();

	void DMA2D_Init(uint32_t ImageWidth, uint32_t ImageHeight);
	void DMA2D_CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize,
						  uint32_t width_offset);
	void DMA2D_IRQHandler(void);

	void LTDCLayerInit(uint16_t layer_index);
	void setActiveLayer(uint32_t layer_index);
	void clear(uint32_t ColorIndex);
	void drawRectangle(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t ColorIndex);
	void drawImage(uint32_t data, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	void fillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine,
					uint32_t ColorIndex);

	void squareBouncing();
	void showFace(bool jpeg_file);

  private:
	SDBlockDevice _interface;
	FATFileSystem _file_interface;
	mbed::DigitalOut _sd_enable;

	uint32_t _active_layer;

	DMA_HandleTypeDef _hdma;
	DMA2D_HandleTypeDef _hdma2d;
};

#endif
