/**
 * @file LekaTouch.h
 * @author Yann Locatelli
 *
 * @version 0.1
 * @date 2020-09-20
 *
 * @copyright Copyright (c) 2020
 */
#ifndef _LEKA_OS_LIB_LEKATOUCH_H_
#define _LEKA_OS_LIB_LEKATOUCH_H_

// #include "LekaTouchPins.h"
#include "LekaPinNames.h"
#include "MCP23017.h"
#include "mbed.h"

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
	void updateSensorsStatus();

  private:
	I2C _write_interface;
	MCP23017 _read_interface;
	DigitalOut _mux_reset;
	DigitalIn _mux_inta;
	DigitalIn _mux_intb;

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

#endif
