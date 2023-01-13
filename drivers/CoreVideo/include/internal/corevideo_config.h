// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreLCDDriverOTM8009A.hpp"

namespace leka {

namespace lcd {

	inline constexpr uintptr_t frame_buffer_address = 0xC0000000;

	// TODO(@leka/dev-embedded): this should not depend on OTM driver,
	// TODO(@leka/dev-embedded): it should be instanciated and passed to objects that need it
	namespace dimension {

		inline constexpr uint16_t width	 = lcd::otm8009a::landscape::width;
		inline constexpr uint16_t height = lcd::otm8009a::landscape::height;

	}	// namespace dimension

	// TODO(@leka/dev-embedded): this should not depend on OTM driver,
	// TODO(@leka/dev-embedded): it should be instanciated and passed to objects that need it
	namespace property {

		inline constexpr uint8_t pixel_memory_size = 4;	  // ARGB, in bytes

		// clang-format off
		inline constexpr uint16_t VSA  = lcd::otm8009a::landscape::vsync;   // Vertical start active time in units of lines
		inline constexpr uint16_t VBP  = lcd::otm8009a::landscape::vbp;	    // Vertical Back Porch time in units of lines
		inline constexpr uint16_t VFP  = lcd::otm8009a::landscape::vfp;	    // Vertical Front Porch time in units of lines
		inline constexpr uint16_t VACT = lcd::otm8009a::landscape::height;	// Vertical Active time in units of lines = imageSize Y in pixels to display

		inline constexpr uint16_t HSA = lcd::otm8009a::landscape::hsync;    // Horizontal start active time in units of lcdClk
		inline constexpr uint16_t HBP = 1;                                  // Horizontal Back Porch time in units of lcdClk  // TODO(@leka/dev-embedded): Determine the correct value to avoid offset
		inline constexpr uint16_t HFP = 1;                                  // Horizontal Front Porch time in units of lcdClk // TODO(@leka/dev-embedded): Determine the correct value to avoid offset
		inline constexpr uint16_t HACT = lcd::otm8009a::landscape::width;	// Horizontal Active time in units of lcdClk = imageSize X in pixels to display
		// clang-format on

	}	// namespace property

}	// namespace lcd

namespace dsi {

	inline constexpr uint32_t lcdClock			= 27429;   // LcdClk = 27429 kHz
	inline constexpr uint32_t laneByteClock_kHz = 62500;   // 500 MHz / 8 = 62.5 MHz = 62500 kHz
	inline constexpr uint32_t txEscapeClockDiv	= laneByteClock_kHz / 15620;

}	// namespace dsi

namespace jpeg {

	inline constexpr uintptr_t decoded_buffer_address =
		lcd::frame_buffer_address + 800UL * 480UL * 4UL;   // --> 0xC0200000;

	namespace mcu {

		inline constexpr uint32_t output_chunk_multiplier = 768;

	}	// namespace mcu

	inline constexpr uint32_t input_chunk_size	= 2048;
	inline constexpr uint32_t output_chunk_size = mcu::output_chunk_multiplier * 4;
	inline constexpr uint32_t input_buffers_nb	= 1;
	inline constexpr uint32_t output_buffers_nb = 8;

	inline constexpr uint8_t JPEG_ARGB8888 = 0;	  // ARGB8888 Color Format
	inline constexpr uint8_t JPEG_RGB888   = 1;	  // RGB888 Color Format
	inline constexpr uint8_t JPEG_RGB565   = 2;	  // RGB565 Color Format

	inline constexpr uint8_t JPEG_RGB_FORMAT = JPEG_ARGB8888;	// Select RGB format: ARGB8888, RGB888, RBG565
	inline constexpr uint8_t JPEG_SWAP_RB	 = 0;				// Change color order to BGR

	inline constexpr uint16_t JPEG_SOI_MARKER = 0xFFD8;	  // JPEG Start of Image marker

}	// namespace jpeg

namespace graphics {
	inline constexpr uint16_t font_pixel_width	= 17;
	inline constexpr uint16_t font_pixel_height = 24;

	inline constexpr uint16_t pixels_per_line = font_pixel_width;			 // 17
	inline constexpr uint16_t bytes_per_line  = (pixels_per_line + 7) / 8;	 // 17 pixels wide fits in 3 bytes
	inline constexpr uint16_t unused_bits	  = 8 * bytes_per_line - pixels_per_line;	// 1 pixel == 1 bit
	inline constexpr uint16_t lines_per_char  = font_pixel_height;						// 24
	inline constexpr uint16_t bytes_per_char  = bytes_per_line * lines_per_char;		// 72 = 3 * 24

}	// namespace graphics

}	// namespace leka
