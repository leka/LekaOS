#include "otm8009a_conf.h"

#include "Screen.h"

// Mandatory by driver otm8009a
void OTM8009A_IO_Delay(uint32_t Delay)
{
	HAL_Delay(Delay);
}

// Mandatory by driver otm8009a
void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams)
{
	leka::DSI_IO_WriteCmd(NbrParams, pParams);
}

// Mandatory by screen
void otm8009RotateUpsideDown(bool upside_down)
{
	uint8_t sens = upside_down ? 0x60 : 0xA0;

	uint8_t ShortRegData39[] = {OTM8009A_CMD_MADCTR, sens};
	uint8_t lcdRegData27[]	 = {0x00, 0x00, 0x03, 0x1F, OTM8009A_CMD_CASET};
	uint8_t lcdRegData28[]	 = {0x00, 0x00, 0x01, 0xDF, OTM8009A_CMD_PASET};

	DSI_IO_WriteCmd(0, (uint8_t *)ShortRegData39);
	DSI_IO_WriteCmd(4, (uint8_t *)lcdRegData27);
	DSI_IO_WriteCmd(4, (uint8_t *)lcdRegData28);
}

// Mandatory by screen
leka::LCD_Model otm8009a_model = {leka::LCD::OTM8009A,	   OTM8009A_800X480_WIDTH, OTM8009A_800X480_HEIGHT,
								  OTM8009A_800X480_HSYNC,  OTM8009A_800X480_HBP,   OTM8009A_800X480_HFP,
								  OTM8009A_800X480_HEIGHT, OTM8009A_800X480_VSYNC, OTM8009A_800X480_VBP,
								  OTM8009A_800X480_VBP,	   OTM8009A_800X480_WIDTH, otm8009RotateUpsideDown};

/**
 * @brief  Switch back on the display if was switched off by previous call of BSP_LCD_DisplayOff().
 *         Exit DSI ULPM mode if was allowed and configured in Dsi Configuration.
 */
void leka::LKScreen::turnOn()	// Previously BSP_LCD_DisplayOn
{
	/* Send Display on DCS command to display */
	HAL_DSI_ShortWrite(&(leka::hdsi), hdsivideo.VirtualChannelID, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_DISPON,
					   0x00);
}

/**
 * @brief  Switch Off the display.
 *         Enter DSI ULPM mode if was allowed and configured in Dsi Configuration.
 */
void leka::LKScreen::turnOff()	 // Previously BSP_LCD_DisplayOff
{
	/* Send Display off DCS Command to display */
	HAL_DSI_ShortWrite(&(leka::hdsi), hdsivideo.VirtualChannelID, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_DISPOFF,
					   0x00);
}

// /**
//  * @brief  Set the brightness value
//  * @param  BrightnessValue: [00: Min (black), 100 Max]
//  */
// void leka::LKScreen::setBrightness(float value)	  // Previously BSP_LCD_SetBrightness
// {
// 	uint8_t BrightnessValue = (uint8_t)(value * 100.0);
// 	/* Send Display on DCS command to display */
// 	HAL_DSI_ShortWrite(&leka::hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_WRDISBV,
// 					   (uint16_t)(BrightnessValue * 255) / 100);
// }
