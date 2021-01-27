// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_SCREEN_H_
#define _LEKA_OS_LIB_SCREEN_H_

#include "PinNames.h"

#include "drivers/PwmOut.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "FATFileSystem.h"
#include "LekaLCD.h"
#include "SDBlockDevice.h"
// #include "decode_dma.h"
#include "Screen.h"
#include "decode_polling.h"
#include "dsi.h"
#include "jpeg_utils.h"
#include "otm8009a.h"
#include "otm8009a_conf.h"
#include "sdram.h"

class Screen
{
  public:
	Screen();
	~Screen() {};

	void start(void);

	int SDInit();
	void getFileSize();

	void JPEGInit();
	void DMA2D_Init(uint32_t ImageWidth, uint32_t ImageHeight);
	void DMA2D_CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize,
						  uint32_t width_offset);
	void DMA2D_IRQHandler(void);
	void JPEG_IRQHandler(void);
	void DMA2_Stream3_IRQHandler(void);
	void DMA2_Stream4_IRQHandler(void);

	void ScreenInit();
	void LCDReset();
	void MSPInit();
	void LTDCInit();

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
	const uint32_t LCD_FRAME_BUFFER		   = 0xC0000000;
	const uint32_t JPEG_OUTPUT_DATA_BUFFER = 0xC0200000;

	LTDC_HandleTypeDef _hltdc;
	SDRAM_HandleTypeDef _hsdram;
	DMA_HandleTypeDef _hdma;
	FMC_SDRAM_CommandTypeDef _hsdramcmd;
	DMA2D_HandleTypeDef _hdma2d;
	JPEG_HandleTypeDef _hjpeg;
	JPEG_ConfTypeDef _hjpeginfo;
};

#endif
