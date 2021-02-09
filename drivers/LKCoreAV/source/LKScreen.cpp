// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKScreen.h"

namespace leka {

using namespace mbed;

LKScreen::LKScreen(PinName backlight) : _brightness(backlight)
{
	_brightness.period(0.01f);	 // Set PWM at 1/(0.01 seconds) = 100Hz
	_brightness = 0.10f;
	// setBrightness(0.10f);
}

void LKScreen::initializeDriver()
{
	OTM8009A_Init(OTM8009A_FORMAT_RGB888, OTM8009A_ORIENTATION_LANDSCAPE);
}

void LKScreen::setBrightness(float value)
{
	if (value > 1.0f) {
		_brightness_value = 1.0f;
	} else if (value < 0) {
		_brightness_value = 0;
	} else {
		_brightness_value = value;
	}
	_brightness = _brightness_value;
}

void LKScreen::turnOff()
{
	/* Send Display off DCS Command to display */
	HAL_DSI_ShortWrite(&(leka::hdsi), hdsivideo.VirtualChannelID, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_DISPOFF,
					   0x00);
}

void LKScreen::turnOn()
{
	/* Send Display on DCS command to display */
	HAL_DSI_ShortWrite(&(leka::hdsi), hdsivideo.VirtualChannelID, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_DISPON,
					   0x00);
}

void LKScreen::rotateUpsideDown(bool upside_down)
{
	uint8_t sens = upside_down ? 0x60 : 0xA0;

	uint8_t ShortRegData39[] = {OTM8009A_CMD_MADCTR, sens};
	uint8_t lcdRegData27[]	 = {0x00, 0x00, 0x03, 0x1F, OTM8009A_CMD_CASET};
	uint8_t lcdRegData28[]	 = {0x00, 0x00, 0x01, 0xDF, OTM8009A_CMD_PASET};

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData39);
	DSI_IO_WriteCmd(4, (uint8_t *)lcdRegData27);
	DSI_IO_WriteCmd(4, (uint8_t *)lcdRegData28);
}

}	// namespace leka

// Mandatory by driver st_otm8009a
void OTM8009A_IO_Delay(uint32_t Delay)
{
	HAL_Delay(Delay);
}
