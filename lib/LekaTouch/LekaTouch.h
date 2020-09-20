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

#include "LekaTouchPins.h"
#include "MCP23017.h"
#include "mbed.h"

class LekaTouch
{
  public:
	LekaTouch();
	~LekaTouch() {};

	void start(void);

  private:
	I2C _write_interface;
	MCP23017 _read_interface;
	DigitalOut _mux_reset;	 // SET TO 1 if necessary
	DigitalIn _mux_inta;
	DigitalIn _mux_intb;

	uint8_t _write_address_left	 = 0xC0;   // To check if left side
	uint8_t _write_address_right = 0xC2;   // To check if right side
	uint8_t _read_address		 = 0x4E;
};

#endif
