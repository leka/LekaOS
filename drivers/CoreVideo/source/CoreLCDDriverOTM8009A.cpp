// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>

#include "rtos/ThisThread.h"

#include "CoreLCDDriverOTM8009A.hpp"

using namespace leka;
using namespace std::chrono_literals;
using namespace lcd::otm8009a;

void CoreLCDDriverOTM8009A::turnOn()
{
	_dsi.write(display::turn_on::array, std::size(display::turn_on::array));
	setBrightness(_previous_brightness_value);
	_backlight.resume();
}

void CoreLCDDriverOTM8009A::turnOff()
{
	_dsi.write(display::turn_off::array, std::size(display::turn_off::array));
	setBrightness(0.F);
	_backlight.suspend();
}

void CoreLCDDriverOTM8009A::setBrightness(float value)
{
	_backlight.write(value);

	if (value != 0.F) {
		_previous_brightness_value = value;
	}
}

void CoreLCDDriverOTM8009A::initialize()
{
	_backlight.period(0.01F);	// Set PWM at 1/(0.01 seconds) = 100Hz

	// Enable CMD2 to access vendor specific commands
	// Enter in command 2 mode and set EXTC to enable address shift function (0x00)
	_dsi.write(register_data::short01, std::size(register_data::short01));
	_dsi.write(register_data::long01, std::size(register_data::long01));

	// Enter ORISE Command 2
	// Shift address to 0x80
	_dsi.write(register_data::short02, std::size(register_data::short02));
	_dsi.write(register_data::long02, std::size(register_data::long02));

	/////////////////////////////////////////////////////////////////////
	// SD_PCH_CTRL - 0xC480h - 129th parameter - Default 0x00
	// Set SD_PT
	// -> Source output level during porch and non-display area to GND
	_dsi.write(register_data::short02, std::size(register_data::short02));
	_dsi.write(register_data::short03, std::size(register_data::short03));

	rtos::ThisThread::sleep_for(10ms);

	// Not documented
	_dsi.write(register_data::short04, std::size(register_data::short04));
	_dsi.write(register_data::short05, std::size(register_data::short05));

	rtos::ThisThread::sleep_for(10ms);

	/////////////////////////////////////////////////////////////////////

	// PWR_CTRL4 - 0xC4B0h - 178th parameter - Default 0xA8
	// Set gvdd_en_test
	// -> enable GVDD test mode !!!
	_dsi.write(register_data::short06, std::size(register_data::short06));
	_dsi.write(register_data::short07, std::size(register_data::short07));

	// PWR_CTRL2 - 0xC590h - 146th parameter - Default 0x79
	// Set pump 4 vgh voltage
	// -> from 15.0v down to 13.0v
	// Set pump 5 vgh voltage
	// -> from -12.0v downto -9.0v
	_dsi.write(register_data::short08, std::size(register_data::short08));
	_dsi.write(register_data::short09, std::size(register_data::short09));

	// P_DRV_M - 0xC0B4h - 181th parameter - Default 0x00
	// -> Column inversion
	_dsi.write(register_data::short10, std::size(register_data::short10));
	_dsi.write(register_data::short11, std::size(register_data::short11));

	// VCOMDC - 0xD900h - 1st parameter - Default 0x39h
	// VCOM Voltage settings
	// -> from -1.0000v downto -1.2625v
	_dsi.write(register_data::short01, std::size(register_data::short01));
	_dsi.write(register_data::short12, std::size(register_data::short12));

	// Oscillator adjustment for Idle/Normal mode (LPDT only) set to 65Hz (default is 60Hz)
	_dsi.write(register_data::short13, std::size(register_data::short13));
	_dsi.write(register_data::short14, std::size(register_data::short14));

	// Video mode internal
	_dsi.write(register_data::short15, std::size(register_data::short15));
	_dsi.write(register_data::short16, std::size(register_data::short16));

	// PWR_CTRL2 - 0xC590h - 147h parameter - Default 0x00
	// Set pump 4&5 x6
	// -> ONLY VALID when PUMP4_EN_ASDM_HV = "0"
	_dsi.write(register_data::short17, std::size(register_data::short17));
	_dsi.write(register_data::short18, std::size(register_data::short18));

	// PWR_CTRL2 - 0xC590h - 150th parameter - Default 0x33h
	// Change pump4 clock ratio
	// -> from 1 line to 1/2 line
	_dsi.write(register_data::short19, std::size(register_data::short19));
	_dsi.write(register_data::short09, std::size(register_data::short09));

	// GVDD/NGVDD settings
	_dsi.write(register_data::short01, std::size(register_data::short01));
	_dsi.write(register_data::long05, std::size(register_data::long05));

	// PWR_CTRL2 - 0xC590h - 149th parameter - Default 0x33h
	// Rewrite the default value !
	_dsi.write(register_data::short20, std::size(register_data::short20));
	_dsi.write(register_data::short21, std::size(register_data::short21));

	// Panel display timing Setting 3
	_dsi.write(register_data::short22, std::size(register_data::short22));
	_dsi.write(register_data::short23, std::size(register_data::short23));

	// Power control 1
	_dsi.write(register_data::short24, std::size(register_data::short24));
	_dsi.write(register_data::short25, std::size(register_data::short25));

	// Source driver precharge
	_dsi.write(register_data::short13, std::size(register_data::short13));
	_dsi.write(register_data::short26, std::size(register_data::short26));

	_dsi.write(register_data::short15, std::size(register_data::short15));
	_dsi.write(register_data::short27, std::size(register_data::short27));

	_dsi.write(register_data::short28, std::size(register_data::short28));
	_dsi.write(register_data::long06, std::size(register_data::long06));

	// GOAVST
	_dsi.write(register_data::short02, std::size(register_data::short02));
	_dsi.write(register_data::long07, std::size(register_data::long07));

	_dsi.write(register_data::short29, std::size(register_data::short29));
	_dsi.write(register_data::long08, std::size(register_data::long08));

	_dsi.write(register_data::short30, std::size(register_data::short30));
	_dsi.write(register_data::long09, std::size(register_data::long09));

	_dsi.write(register_data::short31, std::size(register_data::short31));
	_dsi.write(register_data::long10, std::size(register_data::long10));

	_dsi.write(register_data::short32, std::size(register_data::short32));
	_dsi.write(register_data::short46, std::size(register_data::short46));

	_dsi.write(register_data::short02, std::size(register_data::short02));
	_dsi.write(register_data::long11, std::size(register_data::long11));

	_dsi.write(register_data::short33, std::size(register_data::short33));
	_dsi.write(register_data::long12, std::size(register_data::long12));

	_dsi.write(register_data::short29, std::size(register_data::short29));
	_dsi.write(register_data::long13, std::size(register_data::long13));

	_dsi.write(register_data::short30, std::size(register_data::short30));
	_dsi.write(register_data::long14, std::size(register_data::long14));

	_dsi.write(register_data::short31, std::size(register_data::short31));
	_dsi.write(register_data::long15, std::size(register_data::long15));

	_dsi.write(register_data::short32, std::size(register_data::short32));
	_dsi.write(register_data::long16, std::size(register_data::long16));

	_dsi.write(register_data::short34, std::size(register_data::short34));
	_dsi.write(register_data::long17, std::size(register_data::long17));

	_dsi.write(register_data::short35, std::size(register_data::short35));
	_dsi.write(register_data::long18, std::size(register_data::long18));

	_dsi.write(register_data::short02, std::size(register_data::short02));
	_dsi.write(register_data::long19, std::size(register_data::long19));

	_dsi.write(register_data::short33, std::size(register_data::short33));
	_dsi.write(register_data::long20, std::size(register_data::long20));

	_dsi.write(register_data::short29, std::size(register_data::short29));
	_dsi.write(register_data::long21, std::size(register_data::long21));

	_dsi.write(register_data::short30, std::size(register_data::short30));
	_dsi.write(register_data::long22, std::size(register_data::long22));

	_dsi.write(register_data::short31, std::size(register_data::short31));
	_dsi.write(register_data::long23, std::size(register_data::long23));

	_dsi.write(register_data::short32, std::size(register_data::short32));
	_dsi.write(register_data::long24, std::size(register_data::long24));

	/////////////////////////////////////////////////////////////////////////////
	// PWR_CTRL1 - 0xc580h - 130th parameter - default 0x00
	// Pump 1 min and max DM
	_dsi.write(register_data::short13, std::size(register_data::short13));
	_dsi.write(register_data::short47, std::size(register_data::short47));
	_dsi.write(register_data::short48, std::size(register_data::short48));
	_dsi.write(register_data::short49, std::size(register_data::short49));
	/////////////////////////////////////////////////////////////////////////////

	// CABC LEDPWM frequency adjusted to 19,5kHz
	_dsi.write(register_data::short50, std::size(register_data::short50));
	_dsi.write(register_data::short51, std::size(register_data::short51));

	// Exit CMD2 mode
	_dsi.write(register_data::short01, std::size(register_data::short01));
	_dsi.write(register_data::long25, std::size(register_data::long25));

	/*************************************************************************** */
	// Standard DCS Initialization TO KEEP CAN BE DONE IN HSDT
	/*************************************************************************** */

	// NOP - goes back to DCS std command ?
	_dsi.write(register_data::short01, std::size(register_data::short01));

	// Gamma correction 2.2+ table (HSDT possible)
	_dsi.write(register_data::short01, std::size(register_data::short01));
	_dsi.write(register_data::long03, std::size(register_data::long03));

	// Gamma correction 2.2- table (HSDT possible)
	_dsi.write(register_data::short01, std::size(register_data::short01));
	_dsi.write(register_data::long04, std::size(register_data::long04));

	// Send Sleep Out command to display : no parameter
	_dsi.write(register_data::short36, std::size(register_data::short36));

	// Wait for sleep out exit
	rtos::ThisThread::sleep_for(120ms);

	// Set Pixel color format to RGB888
	_dsi.write(register_data::short38, std::size(register_data::short38));

	// Set orientation to landscape mode
	setLandscapeOrientation();

	// ? CABC : Content Adaptive Backlight Control section start
	// Note : defaut is 0 (lowest Brightness), 0xFF is highest Brightness, try 0x7F : intermediate value
	_dsi.write(register_data::short40, std::size(register_data::short40));

	// defaut is 0, try 0x2C - Brightness Control Block, Display Dimming & BackLight on
	_dsi.write(register_data::short41, std::size(register_data::short41));

	// defaut is 0, try 0x02 - image Content based Adaptive Brightness [Still Picture]
	_dsi.write(register_data::short42, std::size(register_data::short42));

	// defaut is 0 (lowest Brightness), 0xFF is highest Brightness
	_dsi.write(register_data::short43, std::size(register_data::short43));
	// ? CABC : Content Adaptive Backlight Control section end

	// Send Command Display On
	_dsi.write(register_data::short44, std::size(register_data::short44));

	// NOP command
	_dsi.write(register_data::short01, std::size(register_data::short01));

	// Send Command GRAM memory write (no parameters) : this initiates frame write via other DSI commands sent by
	// DSI host from LTDC incoming pixels in video mode
	_dsi.write(register_data::short45, std::size(register_data::short45));
}

void CoreLCDDriverOTM8009A::setLandscapeOrientation()
{
	// ? Following code is implemented based on OTM8009A driver datasheet
	// ? register 36H (Memory Data Access Control)

	auto settings = []() constexpr
	{
		// settings |= std::byte {1 << 7};	  // Set vertical symmetry - needed
		// settings |= std::byte {1 << 5};	  // Set landscape mode - needed

		// settings |= std::byte {1 << 6};	// Set horizontal symmetry - not needed
		// settings |= std::byte {1 << 4};	// Set reverse refresh top to bottom - not needed
		// settings |= std::byte {1 << 3};	// Set use BGR (Blue Green Red) - not needed

		std::byte _settings {0x00};

		auto set_landscape_mode = [&]() constexpr
		{
			_settings |= std::byte {1 << 5};
		};
		auto set_horizontal_symmetry = [&]() constexpr
		{
			_settings |= std::byte {1 << 6};
		};

		set_landscape_mode();
		set_horizontal_symmetry();

		return std::to_integer<uint8_t>(_settings);
	};

	constexpr auto command = std::to_array<uint8_t>({madctr::command, settings()});

	_dsi.write(command.data(), std::size(command));

	//	Set address for columns and pages
	_dsi.write(set_address::for_column::array, std::size(set_address::for_column::array));
	_dsi.write(set_address::for_page::array, std::size(set_address::for_page::array));
}
