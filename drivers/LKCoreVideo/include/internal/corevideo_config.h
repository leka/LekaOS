// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LCD_PROPERTIES_H_
#define _LEKA_OS_LCD_PROPERTIES_H_

#include "st_otm8009a.h"

namespace leka {

namespace lcd {

	constexpr uint32_t frame_buffer_address = 0xC0000000;

	const struct {
		uint16_t width	= OTM8009A_800X480_WIDTH;
		uint16_t height = OTM8009A_800X480_HEIGHT;
	} dimension;

	const struct {
		uint16_t VSA = OTM8009A_800X480_HSYNC;	 // Vertical start active time in units of lines
		uint16_t VBP = OTM8009A_800X480_HBP;	 // Vertical Back Porch time in units of lines
		uint16_t VFP = OTM8009A_800X480_HFP;	 // Vertical Front Porch time in units of lines
		uint16_t VACT =
			OTM8009A_800X480_HEIGHT;   // Vertical Active time in units of lines = imageSize Y in pixels to display
		uint16_t HSA = OTM8009A_800X480_VSYNC;	 // Horizontal start active time in units of lcdClk
		uint16_t HBP = OTM8009A_800X480_VBP;	 // Horizontal Back Porch time in units of lcdClk
		uint16_t HFP = OTM8009A_800X480_VFP;	 // Horizontal Front Porch time in units of lcdClk
		uint16_t HACT =
			OTM8009A_800X480_WIDTH;	  // Horizontal Active time in units of lcdClk = imageSize X in pixels to display
	} property;

}	// namespace lcd

namespace dsi {

	constexpr uint32_t lcdClock			 = 27429;	// LcdClk = 27429 kHz
	constexpr uint32_t laneByteClock_kHz = 62500;	// 500 MHz / 8 = 62.5 MHz = 62500 kHz
	constexpr float txEscapeClockDiv	 = laneByteClock_kHz / 15620;

}	// namespace dsi

namespace jpeg {

	constexpr uint32_t decoded_buffer_address = 0xC0200000;

	constexpr uint32_t input_data_buffer_size = 4096;

	const uint8_t JPEG_ARGB8888 = 0;   // ARGB8888 Color Format
	const uint8_t JPEG_RGB888	= 1;   // RGB888 Color Format
	const uint8_t JPEG_RGB565	= 2;   // RGB565 Color Format

	const uint8_t JPEG_RGB_FORMAT = JPEG_ARGB8888;	 // Select RGB format: ARGB8888, RGB888, RBG565
	const uint8_t JPEG_SWAP_RB	  = 0;				 // Change color order to BGR

	namespace mcu {

		constexpr uint32_t output_data_buffer_size = 768;

	}	// namespace mcu

}	// namespace jpeg

}	// namespace leka

#endif	 // _LEKA_OS_LCD_PROPERTIES_H_
