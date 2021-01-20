#include "dsi.h"
#include "otm8009a.h"
#include "screen.h"
#include "stm32f7xx_hal.h"

/**
 * @brief  OTM8009A delay from BSP of STM32F769I-Discovery
 * @param  Delay: Delay in ms
 */
void OTM8009A_IO_Delay(uint32_t Delay)
{
	HAL_Delay(Delay);
}

void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams)
{
	leka::DSI_IO_WriteCmd(NbrParams, pParams);
}

leka::LCD_Model otm8009a = {leka::LCD::OTM8009A,	 OTM8009A_800X480_WIDTH, OTM8009A_800X480_HEIGHT,
							OTM8009A_800X480_HSYNC,	 OTM8009A_800X480_HBP,	 OTM8009A_800X480_HFP,
							OTM8009A_800X480_HEIGHT, OTM8009A_800X480_VSYNC, OTM8009A_800X480_VBP,
							OTM8009A_800X480_VBP,	 OTM8009A_800X480_WIDTH};
