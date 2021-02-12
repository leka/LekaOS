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
}	// namespace leka

#endif	 // _LEKA_OS_LCD_PROPERTIES_H_
