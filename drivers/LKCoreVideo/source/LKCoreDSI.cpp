// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreDSI.h"

#include "rtos/ThisThread.h"

#include "corevideo_config.h"

using namespace std::chrono;
using namespace leka;

LKCoreDSI::LKCoreDSI(LKCoreSTM32HalBase &hal) : _hal(hal)
{
	_hdsi.Instance			 = DSI;
	_hdsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
	_hdsi.Init.TXEscapeCkdiv = dsi::txEscapeClockDiv;

	_cmdconf.VirtualChannelID	   = 0;
	_cmdconf.HSPolarity			   = DSI_HSYNC_ACTIVE_HIGH;
	_cmdconf.VSPolarity			   = DSI_VSYNC_ACTIVE_HIGH;
	_cmdconf.DEPolarity			   = DSI_DATA_ENABLE_ACTIVE_HIGH;
	_cmdconf.ColorCoding		   = DSI_RGB888;
	_cmdconf.CommandSize		   = lcd::dimension.width / dsi::refresh_columns_count;
	_cmdconf.TearingEffectSource   = DSI_TE_DSILINK;
	_cmdconf.TearingEffectPolarity = DSI_TE_RISING_EDGE;
	_cmdconf.VSyncPol			   = DSI_VSYNC_FALLING;
	_cmdconf.AutomaticRefresh	   = DSI_AR_DISABLE;
	_cmdconf.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_ENABLE;

	for (int i = 0; i < dsi::refresh_columns_count; ++i) {
		auto col_width	= _cmdconf.CommandSize;
		auto col_offset = i * col_width;
		_columns[i][0]	= (col_offset & 0xff00) >> 8;
		_columns[i][1]	= (col_offset & 0x00ff) >> 0;
		_columns[i][2]	= ((col_offset + col_width - 1) & 0xff00) >> 8;
		_columns[i][3]	= ((col_offset + col_width - 1) & 0x00ff) >> 0;
	}
}

void LKCoreDSI::initialize()
{
	reset();

	_hal.HAL_DSI_DeInit(&_hdsi);

	DSI_PLLInitTypeDef dsiPllInit;

	dsiPllInit.PLLNDIV = 100;
	dsiPllInit.PLLIDF  = DSI_PLL_IN_DIV5;
	dsiPllInit.PLLODF  = DSI_PLL_OUT_DIV1;

	// Initialize DSI
	// DO NOT MOVE to the constructor as LCD initialization
	// must be performed in a very specific order
	_hal.HAL_DSI_Init(&_hdsi, &dsiPllInit);

	// Configure DSI Video mode timings
	HAL_DSI_ConfigAdaptedCommandMode(&_hdsi, &_cmdconf);

	_hal.HAL_DSI_Start(&_hdsi);

	DSI_PHY_TimerTypeDef phy_timings;
	phy_timings.ClockLaneHS2LPTime	= 35;
	phy_timings.ClockLaneLP2HSTime	= 35;
	phy_timings.DataLaneHS2LPTime	= 35;
	phy_timings.DataLaneLP2HSTime	= 35;
	phy_timings.DataLaneMaxReadTime = 0;
	phy_timings.StopWaitTime		= 10;
	HAL_DSI_ConfigPhyTimer(&_hdsi, &phy_timings);
}

void LKCoreDSI::refresh()
{
	uint8_t pScanCol[]	= {0x00, 0x0};				/* Scan @ 533 */
	//HAL_DSI_LongWrite(&_hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 2, 0x44, pScanCol);

	if (_hdsi.Lock == HAL_LOCKED) {
		return;
	}

	_hdsi.Lock = HAL_LOCKED;
	_hdsi.Instance->WCR |= DSI_WCR_LTDCEN;
	_hdsi.Lock = HAL_UNLOCKED;
}

auto LKCoreDSI::getSyncProps() -> LKCoreDSI::SyncProps
{
	return {1, 1, lcd::dimension.width / dsi::refresh_columns_count, 1, 1, 1, lcd::dimension.height, 1};
}

void LKCoreDSI::enableLPCmd()
{
	_lpcmd.LPGenShortWriteNoP  = DSI_LP_GSW0P_ENABLE;
	_lpcmd.LPGenShortWriteOneP = DSI_LP_GSW1P_ENABLE;
	_lpcmd.LPGenShortWriteTwoP = DSI_LP_GSW2P_ENABLE;
	_lpcmd.LPGenShortReadNoP   = DSI_LP_GSR0P_ENABLE;
	_lpcmd.LPGenShortReadOneP  = DSI_LP_GSR1P_ENABLE;
	_lpcmd.LPGenShortReadTwoP  = DSI_LP_GSR2P_ENABLE;
	_lpcmd.LPGenLongWrite	   = DSI_LP_GLW_ENABLE;
	_lpcmd.LPDcsShortWriteNoP  = DSI_LP_DSW0P_ENABLE;
	_lpcmd.LPDcsShortWriteOneP = DSI_LP_DSW1P_ENABLE;
	_lpcmd.LPDcsShortReadNoP   = DSI_LP_DSR0P_ENABLE;
	_lpcmd.LPDcsLongWrite	   = DSI_LP_DLW_ENABLE;
	HAL_DSI_ConfigCommand(&_hdsi, &_lpcmd);
}

void LKCoreDSI::disableLPCmd()
{
	_lpcmd.LPGenShortWriteNoP  = DSI_LP_GSW0P_DISABLE;
	_lpcmd.LPGenShortWriteOneP = DSI_LP_GSW1P_DISABLE;
	_lpcmd.LPGenShortWriteTwoP = DSI_LP_GSW2P_DISABLE;
	_lpcmd.LPGenShortReadNoP   = DSI_LP_GSR0P_DISABLE;
	_lpcmd.LPGenShortReadOneP  = DSI_LP_GSR1P_DISABLE;
	_lpcmd.LPGenShortReadTwoP  = DSI_LP_GSR2P_DISABLE;
	_lpcmd.LPGenLongWrite	   = DSI_LP_GLW_DISABLE;
	_lpcmd.LPDcsShortWriteNoP  = DSI_LP_DSW0P_DISABLE;
	_lpcmd.LPDcsShortWriteOneP = DSI_LP_DSW1P_DISABLE;
	_lpcmd.LPDcsShortReadNoP   = DSI_LP_DSR0P_DISABLE;
	_lpcmd.LPDcsLongWrite	   = DSI_LP_DLW_DISABLE;
	HAL_DSI_ConfigCommand(&_hdsi, &_lpcmd);
}

void LKCoreDSI::enableTearingEffectReporting()
{
	HAL_DSI_ConfigFlowControl(&_hdsi, DSI_FLOW_CONTROL_BTA);

	// Enable GPIOJ clock
	__HAL_RCC_GPIOJ_CLK_ENABLE();

	// Configure DSI_TE pin from MB1166 : Tearing effect on separated GPIO from KoD LCD
	// that is mapped on GPIOJ2 as alternate DSI function (DSI_TE)
	// This pin is used only when the LCD and DSI link is configured in command mode
	GPIO_InitTypeDef GPIO_Init_Structure;
	GPIO_Init_Structure.Pin		  = GPIO_PIN_2;
	GPIO_Init_Structure.Mode	  = GPIO_MODE_AF_PP;
	GPIO_Init_Structure.Pull	  = GPIO_NOPULL;
	GPIO_Init_Structure.Speed	  = GPIO_SPEED_HIGH;
	GPIO_Init_Structure.Alternate = GPIO_AF13_DSI;
	HAL_GPIO_Init(GPIOJ, &GPIO_Init_Structure);

	HAL_DSI_Refresh(&_hdsi);
}

void LKCoreDSI::setupPartialRefresh()
{
	uint8_t pColLeft[]	= {0x00, 0x00, 0x01, 0x8F}; //   0 -> 399
	uint8_t pColRight[] = {0x01, 0x90, 0x03, 0x1F}; // 400 -> 799
	uint8_t pPage[]		= {0x00, 0x00, 0x01, 0xDF}; //   0 -> 479

	HAL_DSI_LongWrite(&_hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, 0x2a, _columns[0].data());
	HAL_DSI_LongWrite(&_hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, 0x2b, pPage);
}

void LKCoreDSI::reset(void)
{
	// Reset DSI configuration
	// DO NOT CHANGE this function

	GPIO_InitTypeDef gpio_init_structure;

	_hal.HAL_RCC_GPIOJ_CLK_ENABLE();

	// Configure the GPIO on PJ15 (MIPI_DSI_RESET)
	gpio_init_structure.Pin	  = GPIO_PIN_15;
	gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
	gpio_init_structure.Pull  = GPIO_PULLUP;
	gpio_init_structure.Speed = GPIO_SPEED_HIGH;

	_hal.HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

	// Activate MIPI_DSI_RESET active low
	_hal.HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_15, GPIO_PIN_RESET);

	rtos::ThisThread::sleep_for(20ms);

	// Desactivate MIPI_DSI_RESET
	_hal.HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_15, GPIO_PIN_SET);

	// Wait for 10ms after releasing MIPI_DSI_RESET before sending commands
	rtos::ThisThread::sleep_for(10ms);
}

auto LKCoreDSI::getHandle() -> DSI_HandleTypeDef &
{
	return _hdsi;
}

auto LKCoreDSI::isBusy() -> bool
{
	return _hdsi.State == HAL_DSI_STATE_BUSY;
}

void LKCoreDSI::write(const uint8_t *data, uint32_t size)
{
	if (size <= 2) {
		_hal.HAL_DSI_ShortWrite(&_hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, data[0], data[1]);
	} else {
		_hal.HAL_DSI_LongWrite(&_hdsi, 0, DSI_DCS_LONG_PKT_WRITE, size, data[size - 1], const_cast<uint8_t *>(data));
	}
}
