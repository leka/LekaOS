// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX - License - Identifier : Apache - 2.0

#include "CoreTouch.h"

using namespace leka;

void CoreTouch::init()
{
	_sensors.init(mcp23017::pin::PA0 | mcp23017::pin::PA0 | mcp23017::pin::PA0 | mcp23017::pin::PA0 |
				  mcp23017::pin::PA0 | mcp23017::pin::PA0);
	DACInit();
}

void CoreTouch::DACInit()
{
	_dac_left_sensors.setVoltageReference(dac_touch::data::voltage_reference::Vdd);
	_dac_right_sensors.setVoltageReference(dac_touch::data::voltage_reference::Vdd);

	_dac_left_sensors.setPowerMode(dac_touch::data::power_mode::normal);
	_dac_right_sensors.setPowerMode(dac_touch::data::power_mode::normal);

	_dac_left_sensors.setGain(0x00);
	_dac_right_sensors.setGain(0x00);
}
