// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/PwmOut.h"

#include "interface/DSI.hpp"
#include "interface/LCDDriver.hpp"

namespace leka {

// ? OTM8009A LCD driver datasheet available at:
// ? http://www.orientdisplay.com/pdf/OTM8009A.pdf

class CoreLCDDriverOTM8009A : public interface::LCDDriver
{
  public:
	CoreLCDDriverOTM8009A(interface::DSIBase &dsi, PinName backlight) : _dsi {dsi}, _backlight {backlight} {};

	void initialize() final;
	void setLandscapeOrientation() final;

	void turnOn() final;
	void turnOff() final;

	void setBrightness(float value) final;

  private:
	interface::DSIBase &_dsi;
	mbed::PwmOut _backlight;

	float _previous_brightness_value = {1.F};
};

namespace lcd::otm8009a {

	// #define OTM8009A_480X800_FREQUENCY_DIVIDER 2   // LCD Frequency divider
	constexpr uint32_t frequency_divider = 2;

	namespace orientation {

		// #define OTM8009A_ORIENTATION_PORTRAIT  ((uint32_t)0x00)	  // Portrait orientation choice of LCD screen
		// #define OTM8009A_ORIENTATION_LANDSCAPE ((uint32_t)0x01)	  // Landscape orientation choice of LCD screen
		constexpr uint32_t portait	 = 0x00;
		constexpr uint32_t landscape = 0x01;

	}	// namespace orientation

	namespace format {

		// #define OTM8009A_FORMAT_RGB888 ((uint32_t)0x00)	  // Pixel format chosen is RGB888 : 24 bpp
		// #define OTM8009A_FORMAT_RBG565 ((uint32_t)0x02)	  // Pixel format chosen is RGB565 : 16 bpp
		constexpr uint32_t rgb888 = 0x00;
		constexpr uint32_t rbg565 = 0x02;

	}	// namespace format

	namespace portrait {

		// Width and Height in Portrait mode
		// #define OTM8009A_480X800_WIDTH  ((uint16_t)480)	  // LCD PIXEL WIDTH
		// #define OTM8009A_480X800_HEIGHT ((uint16_t)800)	  // LCD PIXEL HEIGHT
		constexpr uint16_t width  = 480;
		constexpr uint16_t height = 800;

		// Timing parameters for Portrait orientation mode
		// #define OTM8009A_480X800_HSYNC ((uint16_t)2)		// Horizontal synchronization
		// #define OTM8009A_480X800_HBP   ((uint16_t)34)	// Horizontal back porch
		// #define OTM8009A_480X800_HFP   ((uint16_t)34)	// Horizontal front porch
		// #define OTM8009A_480X800_VSYNC ((uint16_t)1)		// Vertical synchronization
		// #define OTM8009A_480X800_VBP   ((uint16_t)15)	// Vertical back porch
		// #define OTM8009A_480X800_VFP   ((uint16_t)16)	// Vertical front porch
		constexpr uint16_t hsync = 2;
		constexpr uint16_t hbp	 = 34;
		constexpr uint16_t hfp	 = 34;
		constexpr uint16_t vsync = 1;
		constexpr uint16_t vbp	 = 15;
		constexpr uint16_t vfp	 = 16;

	}	// namespace portrait

	namespace landscape {

		// Width and Height in Landscape mode
		// #define OTM8009A_800X480_WIDTH	((uint16_t)800)	  // LCD PIXEL WIDTH
		// #define OTM8009A_800X480_HEIGHT ((uint16_t)480)	  // LCD PIXEL HEIGHT
		constexpr uint16_t width  = 800;
		constexpr uint16_t height = 480;

		// Timing parameters for Landscape orientation mode
		// #define OTM8009A_800X480_HSYNC OTM8009A_480X800_VSYNC	// Horizontal synchronization
		// #define OTM8009A_800X480_HBP   OTM8009A_480X800_VBP		// Horizontal back porch
		// #define OTM8009A_800X480_HFP   OTM8009A_480X800_VFP		// Horizontal front porch
		// #define OTM8009A_800X480_VSYNC OTM8009A_480X800_HSYNC	// Vertical synchronization
		// #define OTM8009A_800X480_VBP   OTM8009A_480X800_HBP		// Vertical back porch
		// #define OTM8009A_800X480_VFP   OTM8009A_480X800_HFP		// Vertical front porch
		constexpr uint16_t hsync = portrait::vsync;
		constexpr uint16_t hbp	 = portrait::vbp;
		constexpr uint16_t hfp	 = portrait::vfp;
		constexpr uint16_t vsync = portrait::hsync;
		constexpr uint16_t vbp	 = portrait::hbp;
		constexpr uint16_t vfp	 = portrait::hfp;

	}	// namespace landscape

	namespace nop {

		// #define OTM8009A_CMD_NOP 0x00	// NOP command
		constexpr uint8_t command = 0x00;

	}	// namespace nop

	namespace sleepout {

		// #define OTM8009A_CMD_SLPOUT 0x11   // Sleep Out command
		constexpr uint8_t command = 0x11;

	}	// namespace sleepout

	namespace colormode {

		// #define OTM8009A_CMD_COLMOD 0x3A   // Interface Pixel format command
		constexpr uint8_t command = 0x3A;

		// Possible values of COLMOD parameter corresponding to used pixel formats
		// #define OTM8009A_COLMOD_RGB565 0x55
		// #define OTM8009A_COLMOD_RGB888 0x77
		constexpr uint8_t rgb565 = 0x55;
		constexpr uint8_t rgb888 = 0x77;

	}	// namespace colormode

	namespace madctr {	 // Memory access write control

		// #define OTM8009A_CMD_MADCTR 0x36   // Memory Access write control command
		constexpr uint8_t command = 0x36;

		// Possible used values of MADCTR
		// #define OTM8009A_MADCTR_MODE_PORTRAIT  0x00
		// #define OTM8009A_MADCTR_MODE_LANDSCAPE 0x60	  // MY = 0, MX = 1, MV = 1, ML = 0, RGB = 0
		constexpr uint8_t portrait	= 0x00;
		constexpr uint8_t landscape = 0x60;

	}	// namespace madctr

	namespace display {

		namespace turn_on {

			// #define OTM8009A_CMD_DISPON 0x29   // Display On command
			constexpr uint8_t command = 0x29;
			constexpr uint8_t array[] = {command, 0x00};

		}	// namespace turn_on

		namespace turn_off {

			// #define OTM8009A_CMD_DISPOFF 0x28	// Display Off command
			constexpr uint8_t command = 0x28;
			constexpr uint8_t array[] = {command, 0x00};

		}	// namespace turn_off

	}	// namespace display

	namespace gram::command {

		// #define OTM8009A_CMD_RAMWR 0x2C	  // Memory (GRAM) write command
		// #define OTM8009A_CMD_RAMRD 0x2E	  // Memory (GRAM) read command
		constexpr uint8_t write = 0x2C;
		constexpr uint8_t read	= 0x2E;

	}	// namespace gram::command

	namespace cabc::command {

		// CABC Management : ie : Content Adaptive Back light Control in IC OTM8009a
		// #define OTM8009A_CMD_WRDISBV  0x51	 // Write Display Brightness command
		// #define OTM8009A_CMD_WRCTRLD  0x53	 // Write CTRL Display command
		// #define OTM8009A_CMD_WRCABC	  0x55	 // Write Content Adaptive Brightness command
		// #define OTM8009A_CMD_WRCABCMB 0x5E	 // Write CABC Minimum Brightness command
		constexpr uint8_t wrdisbv  = 0x51;
		constexpr uint8_t wrctrld  = 0x53;
		constexpr uint8_t wrcabc   = 0x55;
		constexpr uint8_t wrcabcmb = 0x5E;

	}	// namespace cabc::command

	namespace set_address {

		namespace for_column {

			// #define OTM8009A_CMD_CASET 0x2A	  // Column address set command
			constexpr uint8_t command = 0x2A;
			constexpr uint8_t array[] = {0x00, 0x00, 0x03, 0x1F, command};

		}	// namespace for_column

		namespace for_page {

			// #define OTM8009A_CMD_PASET 0x2B	  // Page address set command
			constexpr uint8_t command = 0x2B;
			constexpr uint8_t array[] = {0x00, 0x00, 0x01, 0xDF, command};

		}	// namespace for_page

	}	// namespace set_address

	namespace register_data {

		constexpr uint8_t long01[] = {0x80, 0x09, 0x01, 0xFF};
		constexpr uint8_t long02[] = {0x80, 0x09, 0xFF};
		constexpr uint8_t long03[] = {0x00, 0x09, 0x0F, 0x0E, 0x07, 0x10, 0x0B, 0x0A, 0x04,
									  0x07, 0x0B, 0x08, 0x0F, 0x10, 0x0A, 0x01, 0xE1};
		constexpr uint8_t long04[] = {0x00, 0x09, 0x0F, 0x0E, 0x07, 0x10, 0x0B, 0x0A, 0x04,
									  0x07, 0x0B, 0x08, 0x0F, 0x10, 0x0A, 0x01, 0xE2};
		constexpr uint8_t long05[] = {0x79, 0x79, 0xD8};
		constexpr uint8_t long06[] = {0x00, 0x01, 0xB3};
		constexpr uint8_t long07[] = {0x85, 0x01, 0x00, 0x84, 0x01, 0x00, 0xCE};
		constexpr uint8_t long08[] = {0x18, 0x04, 0x03, 0x39, 0x00, 0x00, 0x00, 0x18,
									  0x03, 0x03, 0x3A, 0x00, 0x00, 0x00, 0xCE};
		constexpr uint8_t long09[] = {0x18, 0x02, 0x03, 0x3B, 0x00, 0x00, 0x00, 0x18,
									  0x01, 0x03, 0x3C, 0x00, 0x00, 0x00, 0xCE};
		constexpr uint8_t long10[] = {0x01, 0x01, 0x20, 0x20, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0xCF};
		constexpr uint8_t long11[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB};
		constexpr uint8_t long12[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB};
		constexpr uint8_t long13[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB};
		constexpr uint8_t long14[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB};
		constexpr uint8_t long15[] = {0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00,
									  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB};
		constexpr uint8_t long16[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04,
									  0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0xCB};
		constexpr uint8_t long17[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCB};
		constexpr uint8_t long18[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCB};
		constexpr uint8_t long19[] = {0x00, 0x26, 0x09, 0x0B, 0x01, 0x25, 0x00, 0x00, 0x00, 0x00, 0xCC};
		constexpr uint8_t long20[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									  0x00, 0x00, 0x00, 0x26, 0x0A, 0x0C, 0x02, 0xCC};
		constexpr uint8_t long21[] = {0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC};
		constexpr uint8_t long22[] = {0x00, 0x25, 0x0C, 0x0A, 0x02, 0x26, 0x00, 0x00, 0x00, 0x00, 0xCC};
		constexpr uint8_t long23[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									  0x00, 0x00, 0x00, 0x25, 0x0B, 0x09, 0x01, 0xCC};
		constexpr uint8_t long24[] = {0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC};
		constexpr uint8_t long25[] = {0xFF, 0xFF, 0xFF, 0xFF};

		// CASET value (Column Address Set) : X direction LCD GRAM boundaries
		// depending on LCD orientation mode and PASET value (Page Address Set) : Y direction
		// LCD GRAM boundaries depending on LCD orientation mode
		// XS[15:0] = 0x000 = 0, XE[15:0] = 0x31F = 799 for landscape mode : apply to CASET
		// YS[15:0] = 0x000 = 0, YE[15:0] = 0x31F = 799 for portrait mode  : apply to PASET
		constexpr uint8_t long27[] = {0x00, 0x00, 0x03, 0x1F, set_address::for_column::command};

		// XS[15:0] = 0x000 = 0, XE[15:0] = 0x1DF = 479 for portrait mode  : apply to CASET
		// YS[15:0] = 0x000 = 0, YE[15:0] = 0x1DF = 479 for landscape mode : apply to PASET
		constexpr uint8_t long28[] = {0x00, 0x00, 0x01, 0xDF, set_address::for_page::command};

		constexpr uint8_t short01[] = {nop::command, 0x00};
		constexpr uint8_t short02[] = {nop::command, 0x80};
		constexpr uint8_t short03[] = {0xC4, 0x30};
		constexpr uint8_t short04[] = {nop::command, 0x8A};
		constexpr uint8_t short05[] = {0xC4, 0x40};
		constexpr uint8_t short06[] = {nop::command, 0xB1};
		constexpr uint8_t short07[] = {0xC5, 0xA9};
		constexpr uint8_t short08[] = {nop::command, 0x91};
		constexpr uint8_t short09[] = {0xC5, 0x34};
		constexpr uint8_t short10[] = {nop::command, 0xB4};
		constexpr uint8_t short11[] = {0xC0, 0x50};
		constexpr uint8_t short12[] = {0xD9, 0x4E};
		constexpr uint8_t short13[] = {nop::command, 0x81};
		constexpr uint8_t short14[] = {0xC1, 0x66};
		constexpr uint8_t short15[] = {nop::command, 0xA1};
		constexpr uint8_t short16[] = {0xC1, 0x08};
		constexpr uint8_t short17[] = {nop::command, 0x92};
		constexpr uint8_t short18[] = {0xC5, 0x01};
		constexpr uint8_t short19[] = {nop::command, 0x95};
		constexpr uint8_t short20[] = {nop::command, 0x94};
		constexpr uint8_t short21[] = {0xC5, 0x33};
		constexpr uint8_t short22[] = {nop::command, 0xA3};
		constexpr uint8_t short23[] = {0xC0, 0x1B};
		constexpr uint8_t short24[] = {nop::command, 0x82};
		constexpr uint8_t short25[] = {0xC5, 0x83};
		constexpr uint8_t short26[] = {0xC4, 0x83};
		constexpr uint8_t short27[] = {0xC1, 0x0E};
		constexpr uint8_t short28[] = {nop::command, 0xA6};
		constexpr uint8_t short29[] = {nop::command, 0xA0};
		constexpr uint8_t short30[] = {nop::command, 0xB0};
		constexpr uint8_t short31[] = {nop::command, 0xC0};
		constexpr uint8_t short32[] = {nop::command, 0xD0};
		constexpr uint8_t short33[] = {nop::command, 0x90};
		constexpr uint8_t short34[] = {nop::command, 0xE0};
		constexpr uint8_t short35[] = {nop::command, 0xF0};
		constexpr uint8_t short36[] = {sleepout::command, 0x00};
		constexpr uint8_t short37[] = {colormode::command, colormode::rgb565};
		constexpr uint8_t short38[] = {colormode::command, colormode::rgb888};
		constexpr uint8_t short39[] = {madctr::command, madctr::landscape};
		constexpr uint8_t short40[] = {cabc::command::wrdisbv, 0x7F};
		constexpr uint8_t short41[] = {cabc::command::wrctrld, 0x2C};
		constexpr uint8_t short42[] = {cabc::command::wrcabc, 0x02};
		constexpr uint8_t short43[] = {cabc::command::wrcabcmb, 0xFF};
		constexpr uint8_t short44[] = {display::turn_on::command, 0x00};
		constexpr uint8_t short45[] = {gram::command::write, 0x00};
		constexpr uint8_t short46[] = {0xCF, 0x00};
		constexpr uint8_t short47[] = {0xC5, 0x66};
		constexpr uint8_t short48[] = {nop::command, 0xB6};
		constexpr uint8_t short49[] = {0xF5, 0x06};
		constexpr uint8_t short50[] = {nop::command, 0xB1};
		constexpr uint8_t short51[] = {0xC6, 0x06};

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
