// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreLCD.h"

#include "rtos/ThisThread.h"

#include "st_otm8009a.h"

// Implementation mandatory st_otm8009a driver
void OTM8009A_IO_Delay(uint32_t delay)
{
	rtos::ThisThread::sleep_for(std::chrono::milliseconds(delay));
}

namespace leka {

namespace lcd {

	namespace command {

		namespace status {

			constexpr uint8_t on[2]	 = {OTM8009A_CMD_DISPON, 0x00};
			constexpr uint8_t off[2] = {OTM8009A_CMD_DISPOFF, 0x00};

		}	// namespace status

		namespace orientation {

			constexpr uint8_t update_pages[5]  = {0x00, 0x00, 0x01, 0xDF, OTM8009A_CMD_PASET};
			constexpr uint8_t update_column[5] = {0x00, 0x00, 0x03, 0x1F, OTM8009A_CMD_CASET};

		}	// namespace orientation

	}	// namespace command

}	// namespace lcd

LKCoreLCD::LKCoreLCD(PinName backlight, float brightness) : _brightness(backlight)
{
	_brightness.period(0.01f);	 // Set PWM at 1/(0.01 seconds) = 100Hz
	_brightness.write(brightness);
}

void LKCoreLCD::initialize()
{
	OTM8009A_Init(OTM8009A_FORMAT_RGB888, OTM8009A_ORIENTATION_LANDSCAPE);
	setLandscapeOrientation();
}

void LKCoreLCD::turnOn()
{
	DSI_IO_WriteCmd(0, (uint8_t *)lcd::command::status::on);
}

void LKCoreLCD::turnOff()
{
	DSI_IO_WriteCmd(0, (uint8_t *)lcd::command::status::off);
}

void LKCoreLCD::setBrightness(float value)
{
	_brightness.write(value);
}

void LKCoreLCD::setLandscapeOrientation()
{
	// Following code is defined from datasheet of OTM8009A driver, register 36H (Memory Data Access Control)
	bool vertical_symmetry			   = true;
	bool horizontal_symmetry		   = false;
	bool is_landscape				   = true;
	bool reverse_refresh_top_to_bottom = false;
	bool use_bgr					   = false;

	uint8_t command_byte = 0x00;

	if (vertical_symmetry) {
		command_byte |= (1 << 7);
	}
	if (horizontal_symmetry) {
		command_byte |= (1 << 6);
	}
	if (is_landscape) {
		command_byte |= (1 << 5);
	}
	if (reverse_refresh_top_to_bottom) {
		command_byte |= (1 << 4);
	}
	if (use_bgr) {
		command_byte |= (1 << 3);
	}

	uint8_t command_orientation[] = {OTM8009A_CMD_MADCTR, command_byte};

	DSI_IO_WriteCmd(0, command_orientation);
	DSI_IO_WriteCmd(4, (uint8_t *)lcd::command::orientation::update_column);
	DSI_IO_WriteCmd(4, (uint8_t *)lcd::command::orientation::update_pages);
}

}	// namespace leka
