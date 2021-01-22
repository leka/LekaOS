#include "otm8009a_conf.h"

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