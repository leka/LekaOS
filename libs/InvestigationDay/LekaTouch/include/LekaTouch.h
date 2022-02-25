// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// #include "LekaTouchPins.h"

#include <chrono>

#include "PinNames.h"

#include "drivers/DigitalIn.h"
#include "drivers/DigitalOut.h"
#include "drivers/I2C.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "MCP23017.h"

class Touch
{
  public:
	Touch();
	~Touch() {};

	void start(void);
	void initReadInterface();
	void initWriteInterface(uint8_t address);
	void calibrateTwoSensors(bool &sensor_left, bool &sensor_right, uint8_t channel);
	void calibration();
	void printAllReadInterfaceRegisters();
	void printAllWriteInterfaceRegisters(uint8_t address);
	uint8_t updateSensorsStatus();

	void calibrateEars();
	void calibrateBeltRBLF();
	void calibrateBeltLBRF();

  private:
	mbed::I2C _write_interface;
	MCP23017 _read_interface;
	mbed::DigitalOut _mux_reset;
	mbed::DigitalIn _mux_inta;
	mbed::DigitalIn _mux_intb;

	bool _ear_left_touched;
	bool _ear_right_touched;
	bool _belt_left_back_touched;
	bool _belt_left_front_touched;
	bool _belt_right_back_touched;
	bool _belt_right_front_touched;

	const uint8_t _write_address_left  = 0xC0;
	const uint8_t _write_address_right = 0xC2;
	const uint8_t _read_address		   = 0x4E;
};
