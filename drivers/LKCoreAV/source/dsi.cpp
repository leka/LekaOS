// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "dsi.h"

namespace leka {
DSI_HandleTypeDef hdsi;
DSI_VidCfgTypeDef hdsivideo;
int HAL_error_status = 0;
int DSI_IRQ_counter	 = 0;
namespace dsi {
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

	void init()
	{
		DSI_PLLInitTypeDef dsiPllInit;
		uint32_t LcdClock = 27429; /*!< LcdClk = 27429 kHz */

		uint32_t laneByteClk_kHz = 0;
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

		hdsivideo.VirtualChannelID = 0;			   // Previously LCD_OTM8009A_ID = 0
		hdsivideo.ColorCoding	   = DSI_RGB888;   // Previously LCD_DSI_PIXEL_DATA_FMT_RBG888 = DSI_RGB888
		hdsivideo.VSPolarity	   = DSI_VSYNC_ACTIVE_HIGH;
		hdsivideo.HSPolarity	   = DSI_HSYNC_ACTIVE_HIGH;
		hdsivideo.DEPolarity	   = DSI_DATA_ENABLE_ACTIVE_HIGH;
		hdsivideo.Mode			   = DSI_VID_MODE_BURST; /* Mode Video burst ie : one LgP per line */
		hdsivideo.NullPacketSize   = 0xFFF;
		hdsivideo.NumberOfChunks   = 0;
		hdsivideo.PacketSize =
			screen_property.HACT; /* Value depending on display orientation choice portrait/landscape */
		hdsivideo.HorizontalSyncActive = (screen_property.HSA * laneByteClk_kHz) / LcdClock;
		hdsivideo.HorizontalBackPorch  = (screen_property.HBP * laneByteClk_kHz) / LcdClock;
		hdsivideo.HorizontalLine =
			((screen_property.HACT + screen_property.HSA + screen_property.HBP + screen_property.HFP) *
			 laneByteClk_kHz) /
			LcdClock; /* Value depending on display orientation choice portrait/landscape */
		hdsivideo.VerticalSyncActive = screen_property.VSA;
		hdsivideo.VerticalBackPorch	 = screen_property.VBP;
		hdsivideo.VerticalFrontPorch = screen_property.VFP;
		hdsivideo.VerticalActive =
			screen_property.VACT; /* Value depending on display orientation choice portrait/landscape */

		/* Enable or disable sending LP command while streaming is active in video mode */
		hdsivideo.LPCommandEnable = DSI_LP_COMMAND_ENABLE; /* Enable sending commands in mode LP (Low Power) */

		/* Largest packet size possible to transmit in LP mode in VSA, VBP, VFP regions */
		/* Only useful when sending LP packets is allowed while streaming is active in video mode */
		hdsivideo.LPLargestPacketSize = 16;

		/* Largest packet size possible to transmit in LP mode in HFP region during VACT period */
		/* Only useful when sending LP packets is allowed while streaming is active in video mode */
		hdsivideo.LPVACTLargestPacketSize = 0;

		/* Specify for each region of the video frame, if the transmission of command in LP mode is allowed in this
		 * region
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
	}

	/**
	 * @brief  BSP LCD Reset
	 *         Hw reset the LCD DSI activating its XRES signal (active low for some time)
	 *         and desactivating it later.
	 */
	void reset(void)
	{
		GPIO_InitTypeDef gpio_init_structure;

		__HAL_RCC_GPIOJ_CLK_ENABLE();

		/* Configure the GPIO on PJ15 */
		gpio_init_structure.Pin	  = GPIO_PIN_15;
		gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
		gpio_init_structure.Pull  = GPIO_PULLUP;
		gpio_init_structure.Speed = GPIO_SPEED_HIGH;

		HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

		/* Activate XRES active low */
		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_15, GPIO_PIN_RESET);

		HAL_Delay(20); /* wait 20 ms */

		/* Desactivate XRES */
		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_15, GPIO_PIN_SET);

		/* Wait for 10ms after releasing XRES before sending commands */
		HAL_Delay(10);
	}

	void DSI_IRQHandler(void)
	{
		DSI_IRQ_counter += 1;
		HAL_DSI_IRQHandler(&hdsi);
	}

	void HAL_DSI_ErrorCallback(DSI_HandleTypeDef *hdsi) { HAL_error_status = 1; }

	void start() { HAL_DSI_Start(&hdsi); }
}	// namespace dsi
}	// namespace leka

// Mandatory by driver st_otm8009a
void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams)
{
	leka::dsi::DSI_IO_WriteCmd(NbrParams, pParams);
}

void HAL_DSI_ErrorCallback(DSI_HandleTypeDef *hdsi)
{
	leka::dsi::HAL_DSI_ErrorCallback(hdsi);
}
