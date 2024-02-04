// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>

#include "interface/DSI.hpp"
#include "interface/LCDDriver.hpp"
#include "interface/drivers/PwmOut.h"

namespace leka {

// ? OTM8009A LCD driver datasheet available at:
// ? http://www.orientdisplay.com/pdf/OTM8009A.pdf

class CoreLCDDriverOTM8009A : public interface::LCDDriver
{
  public:
	CoreLCDDriverOTM8009A(interface::DSIBase &dsi, interface::PwmOut &backlight)
		: _dsi {dsi}, _backlight {backlight} {};

	void initialize() final;
	void setLandscapeOrientation() final;

	void turnOn() final;
	void turnOff() final;

	void setBrightness(float value) final;

	void enableDeepSleep() final;
	void disableDeepSleep() final;

  private:
	interface::DSIBase &_dsi;
	interface::PwmOut &_backlight;

	float _previous_brightness_value = {1.F};
};

namespace lcd::otm8009a {

	// #define OTM8009A_480X800_FREQUENCY_DIVIDER 2   // LCD Frequency divider
	inline constexpr auto frequency_divider = uint32_t {2};

	namespace orientation {

		// #define OTM8009A_ORIENTATION_PORTRAIT  ((uint32_t)0x00)	  // Portrait orientation choice of LCD screen
		// #define OTM8009A_ORIENTATION_LANDSCAPE ((uint32_t)0x01)	  // Landscape orientation choice of LCD screen
		inline constexpr auto portait	= uint32_t {0x00};
		inline constexpr auto landscape = uint32_t {0x01};

	}	// namespace orientation

	namespace format {

		// #define OTM8009A_FORMAT_RGB888 ((uint32_t)0x00)	  // Pixel format chosen is RGB888 : 24 bpp
		// #define OTM8009A_FORMAT_RBG565 ((uint32_t)0x02)	  // Pixel format chosen is RGB565 : 16 bpp
		inline constexpr auto rgb888 = uint32_t {0x00};
		inline constexpr auto rbg565 = uint32_t {0x02};

	}	// namespace format

	namespace portrait {

		// Width and Height in Portrait mode
		// #define OTM8009A_480X800_WIDTH  ((uint16_t)480)	  // LCD PIXEL WIDTH
		// #define OTM8009A_480X800_HEIGHT ((uint16_t)800)	  // LCD PIXEL HEIGHT
		inline constexpr auto width	 = uint16_t {480};
		inline constexpr auto height = uint16_t {800};

		// Timing parameters for Portrait orientation mode
		// #define OTM8009A_480X800_HSYNC ((uint16_t)2)		// Horizontal synchronization
		// #define OTM8009A_480X800_HBP   ((uint16_t)34)	// Horizontal back porch
		// #define OTM8009A_480X800_HFP   ((uint16_t)34)	// Horizontal front porch
		// #define OTM8009A_480X800_VSYNC ((uint16_t)1)		// Vertical synchronization
		// #define OTM8009A_480X800_VBP   ((uint16_t)15)	// Vertical back porch
		// #define OTM8009A_480X800_VFP   ((uint16_t)16)	// Vertical front porch
		inline constexpr auto hsync = uint16_t {2};
		inline constexpr auto hbp	= uint16_t {34};
		inline constexpr auto hfp	= uint16_t {34};
		inline constexpr auto vsync = uint16_t {1};
		inline constexpr auto vbp	= uint16_t {15};
		inline constexpr auto vfp	= uint16_t {16};

	}	// namespace portrait

	namespace landscape {

		// Width and Height in Landscape mode
		// #define OTM8009A_800X480_WIDTH	((uint16_t)800)	  // LCD PIXEL WIDTH
		// #define OTM8009A_800X480_HEIGHT ((uint16_t)480)	  // LCD PIXEL HEIGHT
		inline constexpr auto width	 = uint16_t {800};
		inline constexpr auto height = uint16_t {480};

		// Timing parameters for Landscape orientation mode
		// #define OTM8009A_800X480_HSYNC OTM8009A_480X800_VSYNC	// Horizontal synchronization
		// #define OTM8009A_800X480_HBP   OTM8009A_480X800_VBP		// Horizontal back porch
		// #define OTM8009A_800X480_HFP   OTM8009A_480X800_VFP		// Horizontal front porch
		// #define OTM8009A_800X480_VSYNC OTM8009A_480X800_HSYNC	// Vertical synchronization
		// #define OTM8009A_800X480_VBP   OTM8009A_480X800_HBP		// Vertical back porch
		// #define OTM8009A_800X480_VFP   OTM8009A_480X800_HFP		// Vertical front porch
		inline constexpr auto hsync = uint16_t {portrait::vsync};
		inline constexpr auto hbp	= uint16_t {portrait::vbp};
		inline constexpr auto hfp	= uint16_t {portrait::vfp};
		inline constexpr auto vsync = uint16_t {portrait::hsync};
		inline constexpr auto vbp	= uint16_t {portrait::hbp};
		inline constexpr auto vfp	= uint16_t {portrait::hfp};

	}	// namespace landscape

	namespace nop {

		// #define OTM8009A_CMD_NOP 0x00	// NOP command
		inline constexpr auto command = uint8_t {0x00};

	}	// namespace nop

	namespace sleepout {

		// #define OTM8009A_CMD_SLPOUT 0x11   // Sleep Out command
		inline constexpr auto command = uint8_t {0x11};

	}	// namespace sleepout

	namespace colormode {

		// #define OTM8009A_CMD_COLMOD 0x3A   // Interface Pixel format command
		inline constexpr auto command = uint8_t {0x3A};

		// Possible values of COLMOD parameter corresponding to used pixel formats
		// #define OTM8009A_COLMOD_RGB565 0x55
		// #define OTM8009A_COLMOD_RGB888 0x77
		inline constexpr auto rgb565 = uint8_t {0x55};
		inline constexpr auto rgb888 = uint8_t {0x77};

	}	// namespace colormode

	namespace madctr {	 // Memory access write control

		// #define OTM8009A_CMD_MADCTR 0x36   // Memory Access write control command
		inline constexpr auto command = uint8_t {0x36};

		// Possible used values of MADCTR
		// #define OTM8009A_MADCTR_MODE_PORTRAIT  0x00
		// #define OTM8009A_MADCTR_MODE_LANDSCAPE 0x60	  // MY = 0, MX = 1, MV = 1, ML = 0, RGB = 0
		inline constexpr auto portrait	= uint8_t {0x00};
		inline constexpr auto landscape = uint8_t {0x60};

	}	// namespace madctr

	namespace display {

		namespace turn_on {

			// #define OTM8009A_CMD_DISPON 0x29   // Display On command
			inline constexpr auto command = uint8_t {0x29};
			inline constexpr auto array	  = std::to_array<uint8_t>({command, 0x00});

		}	// namespace turn_on

		namespace turn_off {

			// #define OTM8009A_CMD_DISPOFF 0x28	// Display Off command
			inline constexpr auto command = uint8_t {0x28};
			inline constexpr auto array	  = std::to_array<uint8_t>({command, 0x00});

		}	// namespace turn_off

	}	// namespace display

	namespace gram::command {

		// #define OTM8009A_CMD_RAMWR 0x2C	  // Memory (GRAM) write command
		// #define OTM8009A_CMD_RAMRD 0x2E	  // Memory (GRAM) read command
		inline constexpr auto write = uint8_t {0x2C};
		inline constexpr auto read	= uint8_t {0x2E};

	}	// namespace gram::command

	namespace cabc::command {

		// CABC Management : ie : Content Adaptive Back light Control in IC OTM8009a
		// #define OTM8009A_CMD_WRDISBV  0x51	 // Write Display Brightness command
		// #define OTM8009A_CMD_WRCTRLD  0x53	 // Write CTRL Display command
		// #define OTM8009A_CMD_WRCABC	  0x55	 // Write Content Adaptive Brightness command
		// #define OTM8009A_CMD_WRCABCMB 0x5E	 // Write CABC Minimum Brightness command
		inline constexpr auto wrdisbv  = uint8_t {0x51};
		inline constexpr auto wrctrld  = uint8_t {0x53};
		inline constexpr auto wrcabc   = uint8_t {0x55};
		inline constexpr auto wrcabcmb = uint8_t {0x5E};

	}	// namespace cabc::command

	namespace set_address {

		namespace for_column {

			// #define OTM8009A_CMD_CASET 0x2A	  // Column address set command
			inline constexpr auto command = uint8_t {0x2A};
			inline constexpr auto array	  = std::to_array<uint8_t>({0x00, 0x00, 0x03, 0x1F, command});

		}	// namespace for_column

		namespace for_page {

			// #define OTM8009A_CMD_PASET 0x2B	  // Page address set command
			inline constexpr auto command = uint8_t {0x2B};
			inline constexpr auto array	  = std::to_array<uint8_t>({0x00, 0x00, 0x01, 0xDF, command});

		}	// namespace for_page

	}	// namespace set_address

	namespace register_data {

		inline constexpr auto long01 = std::to_array<uint8_t>({0x80, 0x09, 0x01, 0xFF});
		inline constexpr auto long02 = std::to_array<uint8_t>({0x80, 0x09, 0xFF});
		inline constexpr auto long03 = std::to_array<uint8_t>(
			{0x00, 0x09, 0x0F, 0x0E, 0x07, 0x10, 0x0B, 0x0A, 0x04, 0x07, 0x0B, 0x08, 0x0F, 0x10, 0x0A, 0x01, 0xE1});
		inline constexpr auto long04 = std::to_array<uint8_t>(
			{0x00, 0x09, 0x0F, 0x0E, 0x07, 0x10, 0x0B, 0x0A, 0x04, 0x07, 0x0B, 0x08, 0x0F, 0x10, 0x0A, 0x01, 0xE2});
		inline constexpr auto long05 = std::to_array<uint8_t>({0x79, 0x79, 0xD8});
		inline constexpr auto long06 = std::to_array<uint8_t>({0x00, 0x01, 0xB3});
		inline constexpr auto long07 = std::to_array<uint8_t>({0x85, 0x01, 0x00, 0x84, 0x01, 0x00, 0xCE});
		inline constexpr auto long08 = std::to_array<uint8_t>(
			{0x18, 0x04, 0x03, 0x39, 0x00, 0x00, 0x00, 0x18, 0x03, 0x03, 0x3A, 0x00, 0x00, 0x00, 0xCE});
		inline constexpr auto long09 = std::to_array<uint8_t>(
			{0x18, 0x02, 0x03, 0x3B, 0x00, 0x00, 0x00, 0x18, 0x01, 0x03, 0x3C, 0x00, 0x00, 0x00, 0xCE});
		inline constexpr auto long10 =
			std::to_array<uint8_t>({0x01, 0x01, 0x20, 0x20, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0xCF});
		inline constexpr auto long11 =
			std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB});
		inline constexpr auto long12 = std::to_array<uint8_t>(
			{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB});
		inline constexpr auto long13 = std::to_array<uint8_t>(
			{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB});
		inline constexpr auto long14 =
			std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB});
		inline constexpr auto long15 = std::to_array<uint8_t>(
			{0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB});
		inline constexpr auto long16 = std::to_array<uint8_t>(
			{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0xCB});
		inline constexpr auto long17 =
			std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB});
		inline constexpr auto long18 =
			std::to_array<uint8_t>({0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCB});
		inline constexpr auto long19 =
			std::to_array<uint8_t>({0x00, 0x26, 0x09, 0x0B, 0x01, 0x25, 0x00, 0x00, 0x00, 0x00, 0xCC});
		inline constexpr auto long20 = std::to_array<uint8_t>(
			{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x0A, 0x0C, 0x02, 0xCC});
		inline constexpr auto long21 = std::to_array<uint8_t>(
			{0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC});
		inline constexpr auto long22 =
			std::to_array<uint8_t>({0x00, 0x25, 0x0C, 0x0A, 0x02, 0x26, 0x00, 0x00, 0x00, 0x00, 0xCC});
		inline constexpr auto long23 = std::to_array<uint8_t>(
			{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x0B, 0x09, 0x01, 0xCC});
		inline constexpr auto long24 = std::to_array<uint8_t>(
			{0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC});
		inline constexpr auto long25 = std::to_array<uint8_t>({0xFF, 0xFF, 0xFF, 0xFF});

		// CASET value (Column Address Set) : X direction LCD GRAM boundaries
		// depending on LCD orientation mode and PASET value (Page Address Set) : Y direction
		// LCD GRAM boundaries depending on LCD orientation mode
		// XS[15:0] = 0x000 = 0, XE[15:0] = 0x31F = 799 for landscape mode : apply to CASET
		// YS[15:0] = 0x000 = 0, YE[15:0] = 0x31F = 799 for portrait mode  : apply to PASET
		inline constexpr auto long27 =
			std::to_array<uint8_t>({0x00, 0x00, 0x03, 0x1F, set_address::for_column::command});

		// XS[15:0] = 0x000 = 0, XE[15:0] = 0x1DF = 479 for portrait mode  : apply to CASET
		// YS[15:0] = 0x000 = 0, YE[15:0] = 0x1DF = 479 for landscape mode : apply to PASET
		inline constexpr auto long28 = std::to_array<uint8_t>({0x00, 0x00, 0x01, 0xDF, set_address::for_page::command});

		inline constexpr auto short01 = std::to_array<uint8_t>({nop::command, 0x00});
		inline constexpr auto short02 = std::to_array<uint8_t>({nop::command, 0x80});
		inline constexpr auto short03 = std::to_array<uint8_t>({0xC4, 0x30});
		inline constexpr auto short04 = std::to_array<uint8_t>({nop::command, 0x8A});
		inline constexpr auto short05 = std::to_array<uint8_t>({0xC4, 0x40});
		inline constexpr auto short06 = std::to_array<uint8_t>({nop::command, 0xB1});
		inline constexpr auto short07 = std::to_array<uint8_t>({0xC5, 0xA9});
		inline constexpr auto short08 = std::to_array<uint8_t>({nop::command, 0x91});
		inline constexpr auto short09 = std::to_array<uint8_t>({0xC5, 0x34});
		inline constexpr auto short10 = std::to_array<uint8_t>({nop::command, 0xB4});
		inline constexpr auto short11 = std::to_array<uint8_t>({0xC0, 0x50});
		inline constexpr auto short12 = std::to_array<uint8_t>({0xD9, 0x4E});
		inline constexpr auto short13 = std::to_array<uint8_t>({nop::command, 0x81});
		inline constexpr auto short14 = std::to_array<uint8_t>({0xC1, 0x66});
		inline constexpr auto short15 = std::to_array<uint8_t>({nop::command, 0xA1});
		inline constexpr auto short16 = std::to_array<uint8_t>({0xC1, 0x08});
		inline constexpr auto short17 = std::to_array<uint8_t>({nop::command, 0x92});
		inline constexpr auto short18 = std::to_array<uint8_t>({0xC5, 0x01});
		inline constexpr auto short19 = std::to_array<uint8_t>({nop::command, 0x95});
		inline constexpr auto short20 = std::to_array<uint8_t>({nop::command, 0x94});
		inline constexpr auto short21 = std::to_array<uint8_t>({0xC5, 0x33});
		inline constexpr auto short22 = std::to_array<uint8_t>({nop::command, 0xA3});
		inline constexpr auto short23 = std::to_array<uint8_t>({0xC0, 0x1B});
		inline constexpr auto short24 = std::to_array<uint8_t>({nop::command, 0x82});
		inline constexpr auto short25 = std::to_array<uint8_t>({0xC5, 0x83});
		inline constexpr auto short26 = std::to_array<uint8_t>({0xC4, 0x83});
		inline constexpr auto short27 = std::to_array<uint8_t>({0xC1, 0x0E});
		inline constexpr auto short28 = std::to_array<uint8_t>({nop::command, 0xA6});
		inline constexpr auto short29 = std::to_array<uint8_t>({nop::command, 0xA0});
		inline constexpr auto short30 = std::to_array<uint8_t>({nop::command, 0xB0});
		inline constexpr auto short31 = std::to_array<uint8_t>({nop::command, 0xC0});
		inline constexpr auto short32 = std::to_array<uint8_t>({nop::command, 0xD0});
		inline constexpr auto short33 = std::to_array<uint8_t>({nop::command, 0x90});
		inline constexpr auto short34 = std::to_array<uint8_t>({nop::command, 0xE0});
		inline constexpr auto short35 = std::to_array<uint8_t>({nop::command, 0xF0});
		inline constexpr auto short36 = std::to_array<uint8_t>({sleepout::command, 0x00});
		inline constexpr auto short37 = std::to_array<uint8_t>({colormode::command, colormode::rgb565});
		inline constexpr auto short38 = std::to_array<uint8_t>({colormode::command, colormode::rgb888});
		inline constexpr auto short39 = std::to_array<uint8_t>({madctr::command, madctr::landscape});
		inline constexpr auto short40 = std::to_array<uint8_t>({cabc::command::wrdisbv, 0x7F});
		inline constexpr auto short41 = std::to_array<uint8_t>({cabc::command::wrctrld, 0x2C});
		inline constexpr auto short42 = std::to_array<uint8_t>({cabc::command::wrcabc, 0x02});
		inline constexpr auto short43 = std::to_array<uint8_t>({cabc::command::wrcabcmb, 0xFF});
		inline constexpr auto short44 = std::to_array<uint8_t>({display::turn_on::command, 0x00});
		inline constexpr auto short45 = std::to_array<uint8_t>({gram::command::write, 0x00});
		inline constexpr auto short46 = std::to_array<uint8_t>({0xCF, 0x00});
		inline constexpr auto short47 = std::to_array<uint8_t>({0xC5, 0x66});
		inline constexpr auto short48 = std::to_array<uint8_t>({nop::command, 0xB6});
		inline constexpr auto short49 = std::to_array<uint8_t>({0xF5, 0x06});
		inline constexpr auto short50 = std::to_array<uint8_t>({nop::command, 0xB1});
		inline constexpr auto short51 = std::to_array<uint8_t>({0xC6, 0x06});

	}	// namespace register_data

}	// namespace lcd::otm8009a

}	// namespace leka

// ? OTM8009A_CMD not used at the moment

// #define OTM8009A_CMD_SWRESET   0x01 // Sw reset command
// #define OTM8009A_CMD_RDDMADCTL 0x0B // Read Display MADCTR command : read memory display access ctrl
// #define OTM8009A_CMD_RDDCOLMOD 0x0C // Read Display pixel format
// #define OTM8009A_CMD_SLPIN	   0x10 // Sleep In command
// #define OTM8009A_CMD_PTLON 0x12 // Partial mode On command

// #define OTM8009A_CMD_PLTAR 0x30 // Partial area command (4 parameters)

// #define OTM8009A_CMD_TEOFF 0x34 // Tearing Effect Line Off command : command with no parameter
// #define OTM8009A_CMD_TEEON 0x35 // Tearing Effect Line On command : command with 1 parameter 'TELOM'

// // Parameter TELOM : Tearing Effect Line Output Mode : possible values
// #define OTM8009A_TEEON_TELOM_VBLANKING_INFO_ONLY		  0x00
// #define OTM8009A_TEEON_TELOM_VBLANKING_AND_HBLANKING_INFO 0x01

// #define OTM8009A_CMD_IDMOFF 0x38 // Idle mode Off command
// #define OTM8009A_CMD_IDMON	0x39 // Idle mode On command

// #define OTM8009A_CMD_COLMOD 0x3A // Interface Pixel format command

// #define OTM8009A_CMD_RAMWRC 0x3C // Memory write continue command
// #define OTM8009A_CMD_RAMRDC 0x3E // Memory read continue command

// #define OTM8009A_CMD_WRTESCN 0x44 // Write Tearing Effect Scan line command
// #define OTM8009A_CMD_RDSCNL	 0x45 // Read  Tearing Effect Scan line command
