// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "dsi.h"

// #include "otm8009a.h"

namespace leka {
DSI_HandleTypeDef hdsi;
DSI_VidCfgTypeDef hdsivideo;
int HAL_error_status = 0;
int DSI_IRQ_counter	 = 0;

/**
 * @brief  DSI IO write short/long command.
 */
void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams)
{
	if (NbrParams <= 1) {
		HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, pParams[0], pParams[1]);
	} else {
		HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, NbrParams, pParams[NbrParams], pParams);
	}
}

void DSIInit()
{
	// uint32_t VSA  = OTM8009A_480X800_VSYNC; /* 12  */
	// uint32_t VBP  = OTM8009A_480X800_VBP;	/* 12  */
	// uint32_t VFP  = OTM8009A_480X800_VFP;	/* 12  */
	// uint32_t VACT = OTM8009A_800X480_HEIGHT;
	// uint32_t HSA  = OTM8009A_480X800_HSYNC; /* 63  */
	// uint32_t HBP  = OTM8009A_480X800_HBP;	/* 120 */
	// uint32_t HFP  = OTM8009A_480X800_HFP;
	// uint32_t HACT = OTM8009A_800X480_WIDTH;
	uint32_t VSA  = 1;	/* 12  */
	uint32_t VBP  = 15; /* 12  */
	uint32_t VFP  = 16; /* 12  */
	uint32_t VACT = 480;
	uint32_t HSA  = 2;	/* 63  */
	uint32_t HBP  = 34; /* 120 */
	uint32_t HFP  = 34;
	uint32_t HACT = 800;

	DSI_PLLInitTypeDef dsiPllInit;
	uint32_t LcdClock = 27429; /*!< LcdClk = 27429 kHz */

	uint32_t laneByteClk_kHz = 0;
	// uint32_t VSA;  /*!< Vertical start active time in units of lines */
	// uint32_t VBP;  /*!< Vertical Back Porch time in units of lines */
	// uint32_t VFP;  /*!< Vertical Front Porch time in units of lines */
	// uint32_t VACT; /*!< Vertical Active time in units of lines = imageSize Y in pixels to display */
	// uint32_t HSA;  /*!< Horizontal start active time in units of lcdClk */
	// uint32_t HBP;  /*!< Horizontal Back Porch time in units of lcdClk */
	// uint32_t HFP;  /*!< Horizontal Front Porch time in units of lcdClk */
	// uint32_t HACT; /*!< Horizontal Active time in units of lcdClk = imageSize X in pixels to display */

	/* ADDED PARAMETER, NOT PRESENT IN BSP */
	// uint32_t lcd_x_size = OTM8009A_800X480_WIDTH;
	// uint32_t lcd_y_size = OTM8009A_800X480_HEIGHT;
	/* END OF ADDED PARAMETERS */

	/* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
	hdsi.Instance = DSI;

	HAL_DSI_DeInit(&(hdsi));

	dsiPllInit.PLLNDIV = 100;
	dsiPllInit.PLLIDF  = DSI_PLL_IN_DIV5;
	dsiPllInit.PLLODF  = DSI_PLL_OUT_DIV1;
	laneByteClk_kHz	   = 62500; /* 500 MHz / 8 = 62.5 MHz = 62500 kHz */

	/* Set number of Lanes */
	hdsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;

	/* TXEscapeCkdiv = f(LaneByteClk)/15.62 = 4 */
	hdsi.Init.TXEscapeCkdiv = laneByteClk_kHz / 15620;

	HAL_DSI_Init(&(hdsi), &(dsiPllInit));

	/* Timing parameters for all Video modes
	 * Set Timing parameters of LTDC depending on its chosen orientation
	 */
	// if (orientation == LCD_ORIENTATION_PORTRAIT) {
	// 	lcd_x_size = OTM8009A_480X800_WIDTH;  /* 480 */
	// 	lcd_y_size = OTM8009A_480X800_HEIGHT; /* 800 */
	// } else {
	/* lcd_orientation == LCD_ORIENTATION_LANDSCAPE */
	// lcd_x_size = OTM8009A_800X480_WIDTH;  /* 800 */
	// lcd_y_size = OTM8009A_800X480_HEIGHT; /* 480 */
	// }

	// HACT = _screen_width;	 // Previously lcd_x_size, replace by _screen_width
	// VACT = _screen_height;	 // Previously lcd_y_size, replace by _screen_height

	/* The following values are same for portrait and landscape orientations */
	// VSA = OTM8009A_480X800_VSYNC; /* 12  */
	// VBP = OTM8009A_480X800_VBP;	  /* 12  */
	// VFP = OTM8009A_480X800_VFP;	  /* 12  */
	// HSA = OTM8009A_480X800_HSYNC; /* 63  */
	// HBP = OTM8009A_480X800_HBP;	  /* 120 */
	// HFP = OTM8009A_480X800_HFP;	  /* 120 */

	hdsivideo.VirtualChannelID	   = 0;			   // Previously LCD_OTM8009A_ID = 0
	hdsivideo.ColorCoding		   = DSI_RGB888;   // Previously LCD_DSI_PIXEL_DATA_FMT_RBG888 = DSI_RGB888
	hdsivideo.VSPolarity		   = DSI_VSYNC_ACTIVE_HIGH;
	hdsivideo.HSPolarity		   = DSI_HSYNC_ACTIVE_HIGH;
	hdsivideo.DEPolarity		   = DSI_DATA_ENABLE_ACTIVE_HIGH;
	hdsivideo.Mode				   = DSI_VID_MODE_BURST; /* Mode Video burst ie : one LgP per line */
	hdsivideo.NullPacketSize	   = 0xFFF;
	hdsivideo.NumberOfChunks	   = 0;
	hdsivideo.PacketSize		   = HACT; /* Value depending on display orientation choice portrait/landscape */
	hdsivideo.HorizontalSyncActive = (HSA * laneByteClk_kHz) / LcdClock;
	hdsivideo.HorizontalBackPorch  = (HBP * laneByteClk_kHz) / LcdClock;
	hdsivideo.HorizontalLine	   = ((HACT + HSA + HBP + HFP) * laneByteClk_kHz) /
							   LcdClock; /* Value depending on display orientation choice portrait/landscape */
	hdsivideo.VerticalSyncActive = VSA;
	hdsivideo.VerticalBackPorch	 = VBP;
	hdsivideo.VerticalFrontPorch = VFP;
	hdsivideo.VerticalActive	 = VACT; /* Value depending on display orientation choice portrait/landscape */

	/* Enable or disable sending LP command while streaming is active in video mode */
	hdsivideo.LPCommandEnable = DSI_LP_COMMAND_ENABLE; /* Enable sending commands in mode LP (Low Power) */

	/* Largest packet size possible to transmit in LP mode in VSA, VBP, VFP regions */
	/* Only useful when sending LP packets is allowed while streaming is active in video mode */
	hdsivideo.LPLargestPacketSize = 16;

	/* Largest packet size possible to transmit in LP mode in HFP region during VACT period */
	/* Only useful when sending LP packets is allowed while streaming is active in video mode */
	hdsivideo.LPVACTLargestPacketSize = 0;

	/* Specify for each region of the video frame, if the transmission of command in LP mode is allowed in this region
	 */
	/* while streaming is active in video mode */
	hdsivideo.LPHorizontalFrontPorchEnable = DSI_LP_HFP_ENABLE;	 /* Allow sending LP commands during HFP period */
	hdsivideo.LPHorizontalBackPorchEnable  = DSI_LP_HBP_ENABLE;	 /* Allow sending LP commands during HBP period */
	hdsivideo.LPVerticalActiveEnable	   = DSI_LP_VACT_ENABLE; /* Allow sending LP commands during VACT period */
	hdsivideo.LPVerticalFrontPorchEnable   = DSI_LP_VFP_ENABLE;	 /* Allow sending LP commands during VFP period */
	hdsivideo.LPVerticalBackPorchEnable	   = DSI_LP_VBP_ENABLE;	 /* Allow sending LP commands during VBP period */
	hdsivideo.LPVerticalSyncActiveEnable =
		DSI_LP_VSYNC_ENABLE; /* Allow sending LP commands during VSync = VSA period */

	/* Configure DSI Video mode timings with settings set above */
	HAL_DSI_ConfigVideoMode(&(hdsi), &(hdsivideo));

	return;
}

void DSI_IRQHandler(void)
{
	DSI_IRQ_counter += 1;
	HAL_DSI_IRQHandler(&hdsi);
}

void HAL_DSI_ErrorCallback(DSI_HandleTypeDef *hdsi)
{
	HAL_error_status = 1;
}

}	// namespace leka

void HAL_DSI_ErrorCallback(DSI_HandleTypeDef *hdsi)
{
	leka::HAL_DSI_ErrorCallback(hdsi);
}
