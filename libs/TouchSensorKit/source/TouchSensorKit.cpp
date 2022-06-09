// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"

#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

void TouchSensorKit::setup()
{
	set_pull_mode(PinMode::PullUp);
	set_power_mode(touch::power_mode::normal);
	dac_expander_left.setVoltageReference(0x00);
	dac_expander_left.setPowerMode(0x00);
	dac_expander_left.setGain(0x00);
	dac_expander_right.setVoltageReference(0x00);
	dac_expander_right.setPowerMode(0x00);
	dac_expander_right.setGain(0x00);
}

void TouchSensorKit::set_pull_mode(PinMode mode)
{
	_ear_left_input.mode(mode);
	_ear_right_input.mode(mode);
	_belt_left_back_input.mode(mode);
	_belt_left_front_input.mode(mode);
	_belt_right_back_input.mode(mode);
	_belt_right_front_input.mode(mode);
}

void TouchSensorKit::set_power_mode(int power_mode)
{
	_ear_left_pm.write(power_mode);
	_ear_right_pm.write(power_mode);
	_belt_left_back_pm.write(power_mode);
	_belt_left_front_pm.write(power_mode);
	_belt_right_back_pm.write(power_mode);
	_belt_right_front_pm.write(power_mode);
}

void TouchSensorKit::power_mode_reset()
{
	set_power_mode(touch::power_mode::low);
	rtos::ThisThread::sleep_for(5ms);
	set_power_mode(touch::power_mode::normal);
	rtos::ThisThread::sleep_for(5ms);
}

void TouchSensorKit::updateState()
{
	_ear_left_touched		  = (0 == _ear_left_input.read());
	_ear_right_touched		  = (0 == _ear_right_input.read());
	_belt_left_back_touched	  = (0 == _belt_left_back_input.read());
	_belt_left_front_touched  = (0 == _belt_left_front_input.read());
	_belt_right_back_touched  = (0 == _belt_right_back_input.read());
	_belt_right_front_touched = (0 == _belt_right_front_input.read());
	power_mode_reset();
}

void TouchSensorKit::printState()
{
	log_info("Ear left touched: %s", _ear_left_touched ? "true" : "false");
	log_info("Ear right touched: %s", _ear_right_touched ? "true" : "false");
	log_info("Belt left front touched: %s", _belt_left_front_touched ? "true" : "false");
	log_info("Belt left back touched: %s", _belt_left_back_touched ? "true" : "false");
	log_info("Belt right front touched: %s", _belt_right_front_touched ? "true" : "false");
	log_info("Belt right back touched: %s", _belt_right_back_touched ? "true" : "false");
}

void TouchSensorKit::adjust_sensitivity_low()
{
	std::array<uint8_t, 2> value_to_write = {0xFF, 0x0F};
	dac_expander_left.writeToSpecificInputRegister(0, value_to_write);
	dac_expander_left.writeToSpecificInputRegister(0, value_to_write);
	dac_expander_left.writeToSpecificInputRegister(1, value_to_write);
	dac_expander_right.writeToSpecificInputRegister(1, value_to_write);
	dac_expander_right.writeToSpecificInputRegister(2, value_to_write);
	dac_expander_right.writeToSpecificInputRegister(2, value_to_write);
}

void TouchSensorKit::adjust_sensitivity_high()
{
	std::array<uint8_t, 2> value_to_write = {0x01, 0x00};
	dac_expander_left.writeToSpecificInputRegister(0, value_to_write);
	dac_expander_left.writeToSpecificInputRegister(1, value_to_write);
	dac_expander_left.writeToSpecificInputRegister(2, value_to_write);
	dac_expander_right.writeToSpecificInputRegister(0, value_to_write);
	dac_expander_right.writeToSpecificInputRegister(1, value_to_write);
	dac_expander_right.writeToSpecificInputRegister(2, value_to_write);
}

void TouchSensorKit::read_dac_memory(std::array<uint8_t, 24> &value)
{
	dac_expander_left.readMemory(value);
}

// void TouchSensorKit::calibrateTwoSensors(bool &sensor_left, bool &sensor_right, uint8_t channel)
// {
// 	auto value_left_calib  = uint16_t {0x0FFF};
// 	auto value_right_calib = uint16_t {0x0FFF};
// 	auto step			   = uint16_t {0x0001};

// 	dac_expander.reset(CoreDACExpanderMCP4728::_I2C_ADDRESS_LEFT, channel);
// 	dac_expander.reset(CoreDACExpanderMCP4728::_I2C_ADDRESS_RIGHT, channel);

// 	rtos::ThisThread::sleep_for(1s);
// 	updateState();

// 	while (!(sensor_left && sensor_right)) {
// 		if (!sensor_left) {
// 			if (value_left_calib - step > 0x0FFF) {
// 				value_left_calib = 0x0FFF;
// 			} else {
// 				value_left_calib -= step;
// 			}
// 		}
// 		if (!sensor_right) {
// 			if (value_right_calib - step > 0x0FFF) {
// 				value_right_calib = 0x0FFF;
// 			} else {
// 				value_right_calib -= step;
// 			}
// 		}

// 		dac_expander.multiple_write_for_dac_input_registers(CoreDACExpanderMCP4728::_I2C_ADDRESS_LEFT, channel,
// 															value_left_calib);
// 		dac_expander.multiple_write_for_dac_input_registers(CoreDACExpanderMCP4728::_I2C_ADDRESS_RIGHT, channel,
// 															value_right_calib);

// 		rtos::ThisThread::sleep_for(1ms);
// 		updateState();
// 	}

// 	dac_expander.single_write_for_dac_input_register_and_eeprom(CoreDACExpanderMCP4728::_I2C_ADDRESS_LEFT, channel,
// 																value_left_calib);
// 	dac_expander.single_write_for_dac_input_register_and_eeprom(CoreDACExpanderMCP4728::_I2C_ADDRESS_RIGHT, channel,
// 																value_right_calib);
// 	rtos::ThisThread::sleep_for(1ms);

// 	log_info("CALIBRATED!");
// 	rtos::ThisThread::sleep_for(100ms);
// }

// void TouchSensorKit::calibrateEars()
// {
// 	log_info("Place hands on EAR LEFT and EAR RIGHT");
// 	log_info("Calibration will start in 5 seconds");
// 	rtos::ThisThread::sleep_for(5s);
// 	calibrateTwoSensors(_ear_left_touched, _ear_right_touched, 2);
// }

// void TouchSensorKit::calibrateBeltLBRF()
// {
// 	log_info("Place hands on BELT LEFT BACK and BELT RIGHT FRONT");
// 	log_info("Calibration will start in 5 seconds");
// 	rtos::ThisThread::sleep_for(5s);
// 	calibrateTwoSensors(_belt_left_back_touched, _belt_right_front_touched, 1);
// }

// void TouchSensorKit::calibrateBeltRBLF()
// {
// 	log_info("Place hands on BELT LEFT FRONT and BELT RIGHT BACK");
// 	log_info("Calibration will start in 5 seconds");
// 	rtos::ThisThread::sleep_for(5s);
// 	calibrateTwoSensors(_belt_left_front_touched, _belt_right_back_touched, 0);
// }

// void TouchSensorKit::calibration()
// {
// 	log_info("Touch calibration");
// 	log_info("For each of 6 touch sensors, value of sensibility will change");
// 	log_info("Please keep your hands on 2 sensors until \"CALIBRATED !\" appears.\n");
// 	rtos::ThisThread::sleep_for(15s);

// 	calibrateEars();
// 	calibrateBeltLBRF();
// 	calibrateBeltRBLF();
// 	rtos::ThisThread::sleep_for(1s);
// }
