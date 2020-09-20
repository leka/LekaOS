/**
 * @file LekaTouch.cpp
 * @author Yann Locatelli
 *
 * @version 0.1
 * @date 2020-09-20
 *
 * @copyright Copyright (c) 2020
 */

#include "LekaTouch.h"

LekaTouch::LekaTouch()
	: _write_interface(PIN_I2C3_SDA, PIN_I2C3_SCL),
	  _read_interface(PIN_I2C3_SDA, PIN_I2C3_SCL, _read_address),
	  _mux_reset(PIN_MUX_RST),
	  _mux_inta(PIN_MUX_INTA),
	  _mux_intb(PIN_MUX_INTB)
{
}

void LekaTouch::start(void) {}
