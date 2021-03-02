// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>

#include "rtos/ThisThread.h"

#include "LKCoreLCDDriverOTM8009A.h"

namespace leka {

using namespace std::chrono;
using namespace lcd::otm8009a;

LKCoreLCDDriverOTM8009A::LKCoreLCDDriverOTM8009A(LKCoreDSIBase &dsi, PinName backlight)
	: _dsi(dsi), _backlight(backlight)
{
	_backlight.period(0.01f);	// Set PWM at 1/(0.01 seconds) = 100Hz
}

void LKCoreLCDDriverOTM8009A::turnOn()
{
	_dsi.write(display::turn_on::array, std::size(display::turn_on::array));
}

void LKCoreLCDDriverOTM8009A::turnOff()
{
	_dsi.write(display::turn_off::array, std::size(display::turn_off::array));
}

void LKCoreLCDDriverOTM8009A::setBrightness(float brightness)
{
	_backlight.write(brightness);
}

void LKCoreLCDDriverOTM8009A::initialize()
{
	// Enable CMD2 to access vendor specific commands
	// Enter in command 2 mode and set EXTC to enable address shift function (0x00)
	_dsi.write(registerData::short01, std::size(registerData::short01));
	_dsi.write(registerData::long01, std::size(registerData::long01));

	// Enter ORISE Command 2
	// Shift address to 0x80
	_dsi.write(registerData::short02, std::size(registerData::short02));
	_dsi.write(registerData::long02, std::size(registerData::long02));

	/////////////////////////////////////////////////////////////////////
	// SD_PCH_CTRL - 0xC480h - 129th parameter - Default 0x00
	// Set SD_PT
	// -> Source output level during porch and non-display area to GND
	_dsi.write(registerData::short02, std::size(registerData::short02));
	_dsi.write(registerData::short03, std::size(registerData::short03));

	rtos::ThisThread::sleep_for(10ms);

	// Not documented
	_dsi.write(registerData::short04, std::size(registerData::short04));
	_dsi.write(registerData::short05, std::size(registerData::short05));

	rtos::ThisThread::sleep_for(10ms);

	/////////////////////////////////////////////////////////////////////

	// PWR_CTRL4 - 0xC4B0h - 178th parameter - Default 0xA8
	// Set gvdd_en_test
	// -> enable GVDD test mode !!!
	_dsi.write(registerData::short06, std::size(registerData::short06));
	_dsi.write(registerData::short07, std::size(registerData::short07));

	// PWR_CTRL2 - 0xC590h - 146th parameter - Default 0x79
	// Set pump 4 vgh voltage
	// -> from 15.0v down to 13.0v
	// Set pump 5 vgh voltage
	// -> from -12.0v downto -9.0v
	_dsi.write(registerData::short08, std::size(registerData::short08));
	_dsi.write(registerData::short09, std::size(registerData::short09));

	// P_DRV_M - 0xC0B4h - 181th parameter - Default 0x00
	// -> Column inversion
	_dsi.write(registerData::short10, std::size(registerData::short10));
	_dsi.write(registerData::short11, std::size(registerData::short11));

	// VCOMDC - 0xD900h - 1st parameter - Default 0x39h
	// VCOM Voltage settings
	// -> from -1.0000v downto -1.2625v
	_dsi.write(registerData::short01, std::size(registerData::short01));
	_dsi.write(registerData::short12, std::size(registerData::short12));

	// Oscillator adjustment for Idle/Normal mode (LPDT only) set to 65Hz (default is 60Hz)
	_dsi.write(registerData::short13, std::size(registerData::short13));
	_dsi.write(registerData::short14, std::size(registerData::short14));

	// Video mode internal
	_dsi.write(registerData::short15, std::size(registerData::short15));
	_dsi.write(registerData::short16, std::size(registerData::short16));

	// PWR_CTRL2 - 0xC590h - 147h parameter - Default 0x00
	// Set pump 4&5 x6
	// -> ONLY VALID when PUMP4_EN_ASDM_HV = "0"
	_dsi.write(registerData::short17, std::size(registerData::short17));
	_dsi.write(registerData::short18, std::size(registerData::short18));

	// PWR_CTRL2 - 0xC590h - 150th parameter - Default 0x33h
	// Change pump4 clock ratio
	// -> from 1 line to 1/2 line
	_dsi.write(registerData::short19, std::size(registerData::short19));
	_dsi.write(registerData::short09, std::size(registerData::short09));

	// GVDD/NGVDD settings
	_dsi.write(registerData::short01, std::size(registerData::short01));
	_dsi.write(registerData::long05, std::size(registerData::long05));

	// PWR_CTRL2 - 0xC590h - 149th parameter - Default 0x33h
	// Rewrite the default value !
	_dsi.write(registerData::short20, std::size(registerData::short20));
	_dsi.write(registerData::short21, std::size(registerData::short21));

	// Panel display timing Setting 3
	_dsi.write(registerData::short22, std::size(registerData::short22));
	_dsi.write(registerData::short23, std::size(registerData::short23));

	// Power control 1
	_dsi.write(registerData::short24, std::size(registerData::short24));
	_dsi.write(registerData::short25, std::size(registerData::short25));

	// Source driver precharge
	_dsi.write(registerData::short13, std::size(registerData::short13));
	_dsi.write(registerData::short26, std::size(registerData::short26));

	_dsi.write(registerData::short15, std::size(registerData::short15));
	_dsi.write(registerData::short27, std::size(registerData::short27));

	_dsi.write(registerData::short28, std::size(registerData::short28));
	_dsi.write(registerData::long06, std::size(registerData::long06));

	// GOAVST
	_dsi.write(registerData::short02, std::size(registerData::short02));
	_dsi.write(registerData::long07, std::size(registerData::long07));

	_dsi.write(registerData::short29, std::size(registerData::short29));
	_dsi.write(registerData::long08, std::size(registerData::long08));

	_dsi.write(registerData::short30, std::size(registerData::short30));
	_dsi.write(registerData::long09, std::size(registerData::long09));

	_dsi.write(registerData::short31, std::size(registerData::short31));
	_dsi.write(registerData::long10, std::size(registerData::long10));

	_dsi.write(registerData::short32, std::size(registerData::short32));
	_dsi.write(registerData::short46, std::size(registerData::short46));

	_dsi.write(registerData::short02, std::size(registerData::short02));
	_dsi.write(registerData::long11, std::size(registerData::long11));

	_dsi.write(registerData::short33, std::size(registerData::short33));
	_dsi.write(registerData::long12, std::size(registerData::long12));

	_dsi.write(registerData::short29, std::size(registerData::short29));
	_dsi.write(registerData::long13, std::size(registerData::long13));

	_dsi.write(registerData::short30, std::size(registerData::short30));
	_dsi.write(registerData::long14, std::size(registerData::long14));

	_dsi.write(registerData::short31, std::size(registerData::short31));
	_dsi.write(registerData::long15, std::size(registerData::long15));

	_dsi.write(registerData::short32, std::size(registerData::short32));
	_dsi.write(registerData::long16, std::size(registerData::long16));

	_dsi.write(registerData::short34, std::size(registerData::short34));
	_dsi.write(registerData::long17, std::size(registerData::long17));

	_dsi.write(registerData::short35, std::size(registerData::short35));
	_dsi.write(registerData::long18, std::size(registerData::long18));

	_dsi.write(registerData::short02, std::size(registerData::short02));
	_dsi.write(registerData::long19, std::size(registerData::long19));

	_dsi.write(registerData::short33, std::size(registerData::short33));
	_dsi.write(registerData::long20, std::size(registerData::long20));

	_dsi.write(registerData::short29, std::size(registerData::short29));
	_dsi.write(registerData::long21, std::size(registerData::long21));

	_dsi.write(registerData::short30, std::size(registerData::short30));
	_dsi.write(registerData::long22, std::size(registerData::long22));

	_dsi.write(registerData::short31, std::size(registerData::short31));
	_dsi.write(registerData::long23, std::size(registerData::long23));

	_dsi.write(registerData::short32, std::size(registerData::short32));
	_dsi.write(registerData::long24, std::size(registerData::long24));

	/////////////////////////////////////////////////////////////////////////////
	// PWR_CTRL1 - 0xc580h - 130th parameter - default 0x00
	// Pump 1 min and max DM
	_dsi.write(registerData::short13, std::size(registerData::short13));
	_dsi.write(registerData::short47, std::size(registerData::short47));
	_dsi.write(registerData::short48, std::size(registerData::short48));
	_dsi.write(registerData::short49, std::size(registerData::short49));
	/////////////////////////////////////////////////////////////////////////////

	// CABC LEDPWM frequency adjusted to 19,5kHz
	_dsi.write(registerData::short50, std::size(registerData::short50));
	_dsi.write(registerData::short51, std::size(registerData::short51));

	// Exit CMD2 mode
	_dsi.write(registerData::short01, std::size(registerData::short01));
	_dsi.write(registerData::long25, std::size(registerData::long25));

	/*************************************************************************** */
	// Standard DCS Initialization TO KEEP CAN BE DONE IN HSDT
	/*************************************************************************** */

	// NOP - goes back to DCS std command ?
	_dsi.write(registerData::short01, std::size(registerData::short01));

	// Gamma correction 2.2+ table (HSDT possible)
	_dsi.write(registerData::short01, std::size(registerData::short01));
	_dsi.write(registerData::long03, std::size(registerData::long03));

	// Gamma correction 2.2- table (HSDT possible)
	_dsi.write(registerData::short01, std::size(registerData::short01));
	_dsi.write(registerData::long04, std::size(registerData::long04));

	// Send Sleep Out command to display : no parameter
	_dsi.write(registerData::short36, std::size(registerData::short36));

	// Wait for sleep out exit
	rtos::ThisThread::sleep_for(120ms);

	// Set Pixel color format to RGB888
	_dsi.write(registerData::short38, std::size(registerData::short38));

	// Set orientation to landscape mode
	setLandscapeOrientation();

	// ? CABC : Content Adaptive Backlight Control section start
	// Note : defaut is 0 (lowest Brightness), 0xFF is highest Brightness, try 0x7F : intermediate value
	_dsi.write(registerData::short40, std::size(registerData::short40));

	// defaut is 0, try 0x2C - Brightness Control Block, Display Dimming & BackLight on
	_dsi.write(registerData::short41, std::size(registerData::short41));

	// defaut is 0, try 0x02 - image Content based Adaptive Brightness [Still Picture]
	_dsi.write(registerData::short42, std::size(registerData::short42));

	// defaut is 0 (lowest Brightness), 0xFF is highest Brightness
	_dsi.write(registerData::short43, std::size(registerData::short43));
	// ? CABC : Content Adaptive Backlight Control section end

	// Send Command Display On
	_dsi.write(registerData::short44, std::size(registerData::short44));

	// NOP command
	_dsi.write(registerData::short01, std::size(registerData::short01));

	// Send Command GRAM memory write (no parameters) : this initiates frame write via other DSI commands sent by
	// DSI host from LTDC incoming pixels in video mode
	_dsi.write(registerData::short45, std::size(registerData::short45));
}

void LKCoreLCDDriverOTM8009A::setLandscapeOrientation()
{
	// ? Following code is implemented based on OTM8009A driver datasheet
	// ? register 36H (Memory Data Access Control)

	uint8_t settings = 0x00;

	settings |= (1 << 7);	// Set vertical symmetry - needed
	settings |= (1 << 5);	// Set landcape mode - needed

	// settings |= (1 << 6);	// Set horizontal symmetry - not needed
	// settings |= (1 << 4);	// Set reverse refresh top to bottom - not needed
	// settings |= (1 << 3);	// Set use BGR (Blue Green Red) - not needed

	uint8_t command[] = {madctr::command, settings};

	// Set landscape mode
	_dsi.write(command, std::size(command));

	//	Set address for columns and pages
	_dsi.write(set_address::for_column::array, std::size(set_address::for_column::array));
	_dsi.write(set_address::for_page::array, std::size(set_address::for_page::array));
}

}	// namespace leka
