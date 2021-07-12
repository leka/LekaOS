// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LCD_PROPERTIES_H_
#define _LEKA_OS_LCD_PROPERTIES_H_

#include "LKCoreLCDDriverOTM8009A.h"

namespace leka {

namespace lcd {

	constexpr uintptr_t frame_buffer_address  = 0xC0000000;
	constexpr uintptr_t frame_buffer_address2 = 0xC0000000 + 800 * 480 * 4;

	// TODO : this should not depend on OTM driver,
	// TODO : it should be instanciated and passed to objects that need it
	const struct {
		uint16_t width	= lcd::otm8009a::landscape::width;
		uint16_t height = lcd::otm8009a::landscape::height;
	} dimension;

	// TODO : this should not depend on OTM driver,
	// TODO : it should be instanciated and passed to objects that need it
	const struct {
		uint16_t VSA  = lcd::otm8009a::landscape::vsync;	// Vertical start active time in units of lines
		uint16_t VBP  = lcd::otm8009a::landscape::vbp;		// Vertical Back Porch time in units of lines
		uint16_t VFP  = lcd::otm8009a::landscape::vfp;		// Vertical Front Porch time in units of lines
		uint16_t VACT = lcd::otm8009a::landscape::height;	// Vertical Active time in units of lines = imageSize Y in
															// pixels to display
		uint16_t HSA  = lcd::otm8009a::landscape::hsync;	// Horizontal start active time in units of lcdClk
		uint16_t HBP  = lcd::otm8009a::landscape::hbp;		// Horizontal Back Porch time in units of lcdClk
		uint16_t HFP  = lcd::otm8009a::landscape::hfp;		// Horizontal Front Porch time in units of lcdClk
		uint16_t HACT = lcd::otm8009a::landscape::width;   // Horizontal Active time in units of lcdClk = imageSize X in
														   // pixels to display
	} property;

}	// namespace lcd

namespace dsi {

	constexpr uint32_t lcdClock			 = 27429;	// LcdClk = 27429 kHz
	constexpr uint32_t laneByteClock_kHz = 62500;	// 500 MHz / 8 = 62.5 MHz = 62500 kHz
	constexpr uint32_t txEscapeClockDiv	 = laneByteClock_kHz / 15620;

	constexpr uint16_t refresh_columns_count = 1;	// allowed value : 1

}	// namespace dsi

namespace jpeg {

	constexpr uintptr_t decoded_buffer_address = lcd::frame_buffer_address2 + 800 * 480 * 4;   // 0xC0200000;

	constexpr uint32_t chunk_size_in  = 2048;
	constexpr uint32_t chunk_size_out = 768 * 4;
	constexpr uint32_t in_buffers_nb  = 1;
	constexpr uint32_t out_buffers_nb = 8;

	constexpr uint8_t JPEG_ARGB8888 = 0;   // ARGB8888 Color Format
	constexpr uint8_t JPEG_RGB888	= 1;   // RGB888 Color Format
	constexpr uint8_t JPEG_RGB565	= 2;   // RGB565 Color Format

	constexpr uint8_t JPEG_RGB_FORMAT = JPEG_ARGB8888;	 // Select RGB format: ARGB8888, RGB888, RBG565
	constexpr uint8_t JPEG_SWAP_RB	  = 0;				 // Change color order to BGR

	constexpr uint16_t JPEG_SOI_MARKER		 = 0xFFD8;	 // JPEG Start of Image marker
	constexpr uint16_t JPEG_SOI_MARKER_BYTE0 = (JPEG_SOI_MARKER & 0xFF);
	constexpr uint16_t JPEG_SOI_MARKER_BYTE1 = ((JPEG_SOI_MARKER >> 8) & 0xFF);

}	// namespace jpeg

namespace graphics {
	constexpr uint16_t font_pixel_width	 = 17;
	constexpr uint16_t font_pixel_height = 24;

	constexpr uint16_t pixels_per_line = font_pixel_width;						 // 17
	constexpr uint16_t bytes_per_line  = (pixels_per_line + 7) / 8;				 // 17 pixels wide fits in 3 bytes
	constexpr uint16_t unused_bits	   = 8 * bytes_per_line - pixels_per_line;	 // 1 pixel == 1 bit
	constexpr uint16_t lines_per_char  = font_pixel_height;						 // 24
	constexpr uint16_t bytes_per_char  = bytes_per_line * lines_per_char;		 // 72 = 3 * 24

}	// namespace graphics

}	// namespace leka

#endif	 // _LEKA_OS_LCD_PROPERTIES_H_
