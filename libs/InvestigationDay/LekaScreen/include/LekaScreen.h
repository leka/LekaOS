/**
 * @file LekaScreen.h
 * @author Yann Locatelli
 *
 * @version 0.1
 * @date 2020-09-26
 *
 * @copyright Copyright (c) 2020
 */
#ifndef _LEKA_OS_LIB_SCREEN_H_
#define _LEKA_OS_LIB_SCREEN_H_

#include "PinNames.h"

#include "drivers/PwmOut.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "FATFileSystem.h"
#include "LekaLCD.h"
#include "SDBlockDevice.h"
#include "otm8009a.h"

class Screen
{
  public:
	Screen();
	~Screen() {};

	void start(void);

	int SDInit();
	void getFileSize();

	void ScreenInit();
	void LCDReset();
	void MSPInit();
	void DSIInit();
	void LTDCInit();
	void SDRAMInit();
	void SDRAMMSPInit();
	void SDRAMInitSequence();
	void OTM8009AInit();

	void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams);
	uint8_t OTM8009A_Init(uint32_t ColorCoding, uint32_t orientation);

	void LTDCLayerInit(uint16_t layer_index);
	void setActiveLayer(uint32_t layer_index);
	void clear(uint32_t ColorIndex);
	void drawRectangle(uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t ColorIndex);
	void fillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine,
					uint32_t ColorIndex);

	void squareBouncing();

  private:
	mbed::PwmOut _brightness;

	SDBlockDevice _interface;
	FATFileSystem _file_interface;
	mbed::DigitalOut _sd_enable;

	static const uint32_t _screen_width	 = OTM8009A_800X480_WIDTH;
	static const uint32_t _screen_height = OTM8009A_800X480_HEIGHT;

	uint32_t VSA  = OTM8009A_480X800_VSYNC; /* 12  */
	uint32_t VBP  = OTM8009A_480X800_VBP;	/* 12  */
	uint32_t VFP  = OTM8009A_480X800_VFP;	/* 12  */
	uint32_t VACT = OTM8009A_800X480_HEIGHT;
	uint32_t HSA  = OTM8009A_480X800_HSYNC; /* 63  */
	uint32_t HBP  = OTM8009A_480X800_HBP;	/* 120 */
	uint32_t HFP  = OTM8009A_480X800_HFP;
	uint32_t HACT = OTM8009A_800X480_WIDTH;

	uint32_t _active_layer;
	const uint32_t _frame_buffer_start_address = 0xC0000000;

	DSI_HandleTypeDef _hdsi;
	DSI_VidCfgTypeDef _hdsivideo;
	LTDC_HandleTypeDef _hltdc;
	SDRAM_HandleTypeDef _hsdram;
	DMA_HandleTypeDef _hdma;
	FMC_SDRAM_CommandTypeDef _hsdramcmd;
	DMA2D_HandleTypeDef _hdma2d;
};

#endif
