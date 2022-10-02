/**
 ******************************************************************************
 * @file    otm8009a.h
 * @author  MCD Application Team
 * @brief   This file contains all the constants parameters for the OTM8009A
 *          which is the LCD Driver for KoD KM-040TMP-02-0621 (WVGA)
 *          DSI LCD Display.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OTM8009A_H
	#define __OTM8009A_H
	//#include "mbed-os/targets/TARGET_STM/TARGET_STM32F7/STM32Cube_FW/STM32F7xx_HAL_Driver/stm32f7xx_hal.h"
	#include "stm32f7xx_hal.h"

	#ifdef __cplusplus
extern "C" {
	#endif
	/* Includes ------------------------------------------------------------------*/
	#include <stdint.h>
/** @addtogroup BSP
 * @{
 */

/** @addtogroup Components
 * @{
 */

/** @addtogroup otm8009a
 * @{
 */

/** @addtogroup OTM8009A_Exported_Variables
 * @{
 */

	#if defined(__GNUC__)
		#ifndef __weak
			#define __weak __attribute__((weak))
		#endif /* __weak */
	#endif	   /* __GNUC__ */

	/**
	 *  @brief LCD_OrientationTypeDef
	 *  Possible values of Display Orientation
	 */
	#define OTM8009A_ORIENTATION_PORTRAIT  ((uint32_t)0x00) /* Portrait orientation choice of LCD screen  */
	#define OTM8009A_ORIENTATION_LANDSCAPE ((uint32_t)0x01) /* Landscape orientation choice of LCD screen */

	/**
	 *  @brief  Possible values of
	 *  pixel data format (ie color coding) transmitted on DSI Data lane in DSI packets
	 */
	#define OTM8009A_FORMAT_RGB888 ((uint32_t)0x00) /* Pixel format chosen is RGB888 : 24 bpp */
	#define OTM8009A_FORMAT_RBG565 ((uint32_t)0x02) /* Pixel format chosen is RGB565 : 16 bpp */

	/**
	 * @brief  otm8009a_480x800 Size
	 */

	/* Width and Height in Portrait mode */
	#define OTM8009A_480X800_WIDTH	((uint16_t)480) /* LCD PIXEL WIDTH   */
	#define OTM8009A_480X800_HEIGHT ((uint16_t)800) /* LCD PIXEL HEIGHT  */

	/* Width and Height in Landscape mode */
	#define OTM8009A_800X480_WIDTH	((uint16_t)800) /* LCD PIXEL WIDTH   */
	#define OTM8009A_800X480_HEIGHT ((uint16_t)480) /* LCD PIXEL HEIGHT  */

	/**
	 * @brief  OTM8009A_480X800 Timing parameters for Portrait orientation mode
	 */
	#define OTM8009A_480X800_HSYNC ((uint16_t)2)  /* Horizontal synchronization */
	#define OTM8009A_480X800_HBP   ((uint16_t)34) /* Horizontal back porch      */
	#define OTM8009A_480X800_HFP   ((uint16_t)34) /* Horizontal front porch     */
	#define OTM8009A_480X800_VSYNC ((uint16_t)1)  /* Vertical synchronization   */
	#define OTM8009A_480X800_VBP   ((uint16_t)15) /* Vertical back porch        */
	#define OTM8009A_480X800_VFP   ((uint16_t)16) /* Vertical front porch       */

	/**
	 * @brief  OTM8009A_800X480 Timing parameters for Landscape orientation mode
	 *         Same values as for Portrait mode in fact.
	 */
	#define OTM8009A_800X480_HSYNC OTM8009A_480X800_VSYNC /* Horizontal synchronization */
	#define OTM8009A_800X480_HBP   OTM8009A_480X800_VBP	  /* Horizontal back porch      */
	#define OTM8009A_800X480_HFP   OTM8009A_480X800_VFP	  /* Horizontal front porch     */
	#define OTM8009A_800X480_VSYNC OTM8009A_480X800_HSYNC /* Vertical synchronization   */
	#define OTM8009A_800X480_VBP   OTM8009A_480X800_HBP	  /* Vertical back porch        */
	#define OTM8009A_800X480_VFP   OTM8009A_480X800_HFP	  /* Vertical front porch       */

	/* List of OTM8009A used commands                                  */
	/* Detailed in OTM8009A Data Sheet 'DATA_SHEET_OTM8009A_V0 92.pdf' */
	/* Version of 14 June 2012                                         */
	#define OTM8009A_CMD_NOP	   0x00 /* NOP command      */
	#define OTM8009A_CMD_SWRESET   0x01 /* Sw reset command */
	#define OTM8009A_CMD_RDDMADCTL 0x0B /* Read Display MADCTR command : read memory display access ctrl */
	#define OTM8009A_CMD_RDDCOLMOD 0x0C /* Read Display pixel format */
	#define OTM8009A_CMD_SLPIN	   0x10 /* Sleep In command */
	#define OTM8009A_CMD_SLPOUT	   0x11 /* Sleep Out command */
	#define OTM8009A_CMD_PTLON	   0x12 /* Partial mode On command */

	#define OTM8009A_CMD_DISPOFF 0x28 /* Display Off command */
	#define OTM8009A_CMD_DISPON	 0x29 /* Display On command */

	#define OTM8009A_CMD_CASET 0x2A /* Column address set command */
	#define OTM8009A_CMD_PASET 0x2B /* Page address set command */

	#define OTM8009A_CMD_RAMWR 0x2C /* Memory (GRAM) write command */
	#define OTM8009A_CMD_RAMRD 0x2E /* Memory (GRAM) read command  */

	#define OTM8009A_CMD_PLTAR 0x30 /* Partial area command (4 parameters) */

	#define OTM8009A_CMD_TEOFF 0x34 /* Tearing Effect Line Off command : command with no parameter */

	#define OTM8009A_CMD_TEEON 0x35 /* Tearing Effect Line On command : command with 1 parameter 'TELOM' */

	/* Parameter TELOM : Tearing Effect Line Output Mode : possible values */
	#define OTM8009A_TEEON_TELOM_VBLANKING_INFO_ONLY		  0x00
	#define OTM8009A_TEEON_TELOM_VBLANKING_AND_HBLANKING_INFO 0x01

	#define OTM8009A_CMD_MADCTR 0x36 /* Memory Access write control command  */

	/* Possible used values of MADCTR */
	#define OTM8009A_MADCTR_MODE_PORTRAIT  0x00
	#define OTM8009A_MADCTR_MODE_LANDSCAPE 0xA0 /* MY = 0, MX = 1, MV = 1, ML = 0, RGB = 0 */

	#define OTM8009A_CMD_IDMOFF 0x38 /* Idle mode Off command */
	#define OTM8009A_CMD_IDMON	0x39 /* Idle mode On command  */

	#define OTM8009A_CMD_COLMOD 0x3A /* Interface Pixel format command */

	/* Possible values of COLMOD parameter corresponding to used pixel formats */
	#define OTM8009A_COLMOD_RGB565 0x55
	#define OTM8009A_COLMOD_RGB888 0x77

	#define OTM8009A_CMD_RAMWRC 0x3C /* Memory write continue command */
	#define OTM8009A_CMD_RAMRDC 0x3E /* Memory read continue command  */

	#define OTM8009A_CMD_WRTESCN 0x44 /* Write Tearing Effect Scan line command */
	#define OTM8009A_CMD_RDSCNL	 0x45 /* Read  Tearing Effect Scan line command */

	/* CABC Management : ie : Content Adaptive Back light Control in IC OTM8009a */
	#define OTM8009A_CMD_WRDISBV  0x51 /* Write Display Brightness command          */
	#define OTM8009A_CMD_WRCTRLD  0x53 /* Write CTRL Display command                */
	#define OTM8009A_CMD_WRCABC	  0x55 /* Write Content Adaptive Brightness command */
	#define OTM8009A_CMD_WRCABCMB 0x5E /* Write CABC Minimum Brightness command     */

	/**
	 * @brief  OTM8009A_480X800 frequency divider
	 */
	#define OTM8009A_480X800_FREQUENCY_DIVIDER 2 /* LCD Frequency divider      */

/*
 * @brief Constant tables of register settings used to transmit DSI
 * command packets as power up initialization sequence of the KoD LCD (OTM8009A LCD Driver)
 */
const uint8_t lcdRegData1[]	 = {0x80, 0x09, 0x01, 0xFF};
const uint8_t lcdRegData2[]	 = {0x80, 0x09, 0xFF};
const uint8_t lcdRegData3[]	 = {0x00, 0x09, 0x0F, 0x0E, 0x07, 0x10, 0x0B, 0x0A, 0x04,
								0x07, 0x0B, 0x08, 0x0F, 0x10, 0x0A, 0x01, 0xE1};
const uint8_t lcdRegData4[]	 = {0x00, 0x09, 0x0F, 0x0E, 0x07, 0x10, 0x0B, 0x0A, 0x04,
								0x07, 0x0B, 0x08, 0x0F, 0x10, 0x0A, 0x01, 0xE2};
const uint8_t lcdRegData5[]	 = {0x79, 0x79, 0xD8};
const uint8_t lcdRegData6[]	 = {0x00, 0x01, 0xB3};
const uint8_t lcdRegData7[]	 = {0x85, 0x01, 0x00, 0x84, 0x01, 0x00, 0xCE};
const uint8_t lcdRegData8[]	 = {0x18, 0x04, 0x03, 0x39, 0x00, 0x00, 0x00, 0x18,
								0x03, 0x03, 0x3A, 0x00, 0x00, 0x00, 0xCE};
const uint8_t lcdRegData9[]	 = {0x18, 0x02, 0x03, 0x3B, 0x00, 0x00, 0x00, 0x18,
								0x01, 0x03, 0x3C, 0x00, 0x00, 0x00, 0xCE};
const uint8_t lcdRegData10[] = {0x01, 0x01, 0x20, 0x20, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0xCF};
const uint8_t lcdRegData11[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB};
const uint8_t lcdRegData12[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB};
const uint8_t lcdRegData13[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB};
const uint8_t lcdRegData14[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB};
const uint8_t lcdRegData15[] = {0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB};
const uint8_t lcdRegData16[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04,
								0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0xCB};
const uint8_t lcdRegData17[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB};
const uint8_t lcdRegData18[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCB};
const uint8_t lcdRegData19[] = {0x00, 0x26, 0x09, 0x0B, 0x01, 0x25, 0x00, 0x00, 0x00, 0x00, 0xCC};
const uint8_t lcdRegData20[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x26, 0x0A, 0x0C, 0x02, 0xCC};
const uint8_t lcdRegData21[] = {0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC};
const uint8_t lcdRegData22[] = {0x00, 0x25, 0x0C, 0x0A, 0x02, 0x26, 0x00, 0x00, 0x00, 0x00, 0xCC};
const uint8_t lcdRegData23[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x25, 0x0B, 0x09, 0x01, 0xCC};
const uint8_t lcdRegData24[] = {0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC};
const uint8_t lcdRegData25[] = {0xFF, 0xFF, 0xFF, 0xFF};
/*
 * CASET value (Column Address Set) : X direction LCD GRAM boundaries
 * depending on LCD orientation mode and PASET value (Page Address Set) : Y direction
 * LCD GRAM boundaries depending on LCD orientation mode
 * XS[15:0] = 0x000 = 0, XE[15:0] = 0x31F = 799 for landscape mode : apply to CASET
 * YS[15:0] = 0x000 = 0, YE[15:0] = 0x31F = 799 for portrait mode : : apply to PASET
 */
const uint8_t lcdRegData27[] = {0x00, 0x00, 0x03, 0x1F, OTM8009A_CMD_CASET};
// const uint8_t lcdRegData27[] = {0x03, 0x1F, 0x00, 0x00, OTM8009A_CMD_CASET};

/*
 * XS[15:0] = 0x000 = 0, XE[15:0] = 0x1DF = 479 for portrait mode : apply to CASET
 * YS[15:0] = 0x000 = 0, YE[15:0] = 0x1DF = 479 for landscape mode : apply to PASET
 */
const uint8_t lcdRegData28[] = {0x00, 0x00, 0x01, 0xDF, OTM8009A_CMD_PASET};
// const uint8_t lcdRegData28[] = {0x01, 0xDF, 0x00, 0x00, OTM8009A_CMD_PASET};

const uint8_t ShortRegData1[]  = {OTM8009A_CMD_NOP, 0x00};
const uint8_t ShortRegData2[]  = {OTM8009A_CMD_NOP, 0x80};
const uint8_t ShortRegData3[]  = {0xC4, 0x30};
const uint8_t ShortRegData4[]  = {OTM8009A_CMD_NOP, 0x8A};
const uint8_t ShortRegData5[]  = {0xC4, 0x40};
const uint8_t ShortRegData6[]  = {OTM8009A_CMD_NOP, 0xB1};
const uint8_t ShortRegData7[]  = {0xC5, 0xA9};
const uint8_t ShortRegData8[]  = {OTM8009A_CMD_NOP, 0x91};
const uint8_t ShortRegData9[]  = {0xC5, 0x34};
const uint8_t ShortRegData10[] = {OTM8009A_CMD_NOP, 0xB4};
const uint8_t ShortRegData11[] = {0xC0, 0x50};
const uint8_t ShortRegData12[] = {0xD9, 0x4E};
const uint8_t ShortRegData13[] = {OTM8009A_CMD_NOP, 0x81};
const uint8_t ShortRegData14[] = {0xC1, 0x66};
const uint8_t ShortRegData15[] = {OTM8009A_CMD_NOP, 0xA1};
const uint8_t ShortRegData16[] = {0xC1, 0x08};
const uint8_t ShortRegData17[] = {OTM8009A_CMD_NOP, 0x92};
const uint8_t ShortRegData18[] = {0xC5, 0x01};
const uint8_t ShortRegData19[] = {OTM8009A_CMD_NOP, 0x95};
const uint8_t ShortRegData20[] = {OTM8009A_CMD_NOP, 0x94};
const uint8_t ShortRegData21[] = {0xC5, 0x33};
const uint8_t ShortRegData22[] = {OTM8009A_CMD_NOP, 0xA3};
const uint8_t ShortRegData23[] = {0xC0, 0x1B};
const uint8_t ShortRegData24[] = {OTM8009A_CMD_NOP, 0x82};
const uint8_t ShortRegData25[] = {0xC5, 0x83};
const uint8_t ShortRegData26[] = {0xC4, 0x83};
const uint8_t ShortRegData27[] = {0xC1, 0x0E};
const uint8_t ShortRegData28[] = {OTM8009A_CMD_NOP, 0xA6};
const uint8_t ShortRegData29[] = {OTM8009A_CMD_NOP, 0xA0};
const uint8_t ShortRegData30[] = {OTM8009A_CMD_NOP, 0xB0};
const uint8_t ShortRegData31[] = {OTM8009A_CMD_NOP, 0xC0};
const uint8_t ShortRegData32[] = {OTM8009A_CMD_NOP, 0xD0};
const uint8_t ShortRegData33[] = {OTM8009A_CMD_NOP, 0x90};
const uint8_t ShortRegData34[] = {OTM8009A_CMD_NOP, 0xE0};
const uint8_t ShortRegData35[] = {OTM8009A_CMD_NOP, 0xF0};
const uint8_t ShortRegData36[] = {OTM8009A_CMD_SLPOUT, 0x00};
const uint8_t ShortRegData37[] = {OTM8009A_CMD_COLMOD, OTM8009A_COLMOD_RGB565};
const uint8_t ShortRegData38[] = {OTM8009A_CMD_COLMOD, OTM8009A_COLMOD_RGB888};
const uint8_t ShortRegData39[] = {OTM8009A_CMD_MADCTR, OTM8009A_MADCTR_MODE_LANDSCAPE};
const uint8_t ShortRegData40[] = {OTM8009A_CMD_WRDISBV, 0x7F};
const uint8_t ShortRegData41[] = {OTM8009A_CMD_WRCTRLD, 0x2C};
const uint8_t ShortRegData42[] = {OTM8009A_CMD_WRCABC, 0x02};
const uint8_t ShortRegData43[] = {OTM8009A_CMD_WRCABCMB, 0xFF};
const uint8_t ShortRegData44[] = {OTM8009A_CMD_DISPON, 0x00};
const uint8_t ShortRegData45[] = {OTM8009A_CMD_RAMWR, 0x00};
const uint8_t ShortRegData46[] = {0xCF, 0x00};
const uint8_t ShortRegData47[] = {0xC5, 0x66};
const uint8_t ShortRegData48[] = {OTM8009A_CMD_NOP, 0xB6};
const uint8_t ShortRegData49[] = {0xF5, 0x06};
const uint8_t ShortRegData50[] = {OTM8009A_CMD_NOP, 0xB1};
const uint8_t ShortRegData51[] = {0xC6, 0x06};
	/**
	 * @}
	 */
	/**
	 * @}
	 */

	/* Exported macro ------------------------------------------------------------*/

	/** @defgroup OTM8009A_Exported_Macros OTM8009A Exported Macros
	 * @{
	 */

	/**
	 * @}
	 */

	/* Exported functions --------------------------------------------------------*/

	/** @addtogroup OTM8009A_Exported_Functions
	 * @{
	 */
	/*void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams);
	uint8_t OTM8009A_Init(uint32_t ColorCoding, uint32_t orientation);
	void OTM8009A_IO_Delay(uint32_t Delay);*/
	/**
	 * @}
	 */
	#ifdef __cplusplus
}
	#endif

#endif /* __OTM8009A_480X800_H */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
