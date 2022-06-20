// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"

#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

void TouchSensorKit::setup()
{
	setPullMode(PinMode::PullUp);
	setPowerMode(touch::power_mode::normal);
	initDACTouch();
}

void TouchSensorKit::updateState()
{
	_ear_left_touched		  = (0 == _ear_left_input.read());
	_ear_right_touched		  = (0 == _ear_right_input.read());
	_belt_left_back_touched	  = (0 == _belt_left_back_input.read());
	_belt_left_front_touched  = (0 == _belt_left_front_input.read());
	_belt_right_back_touched  = (0 == _belt_right_back_input.read());
	_belt_right_front_touched = (0 == _belt_right_front_input.read());
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

void TouchSensorKit::resetByPowerMode()
{
	setPowerMode(touch::power_mode::low);
	rtos::ThisThread::sleep_for(5ms);
	setPowerMode(touch::power_mode::normal);
	rtos::ThisThread::sleep_for(5ms);
}

void TouchSensorKit::setPullMode(PinMode mode)
{
	_ear_left_input.mode(mode);
	_ear_right_input.mode(mode);
	_belt_left_back_input.mode(mode);
	_belt_left_front_input.mode(mode);
	_belt_right_back_input.mode(mode);
	_belt_right_front_input.mode(mode);
}

void TouchSensorKit::setPowerMode(int power_mode)
{
	_ear_left_pm.write(power_mode);
	_ear_right_pm.write(power_mode);
	_belt_left_back_pm.write(power_mode);
	_belt_left_front_pm.write(power_mode);
	_belt_right_back_pm.write(power_mode);
	_belt_right_front_pm.write(power_mode);
}

void TouchSensorKit::initDACTouch()
{
	dac_expander_left.writeVoltageReference(0x00);
	dac_expander_left.writePowerMode(0x00);
	dac_expander_left.writeGain(0x00);
	dac_expander_right.writeVoltageReference(0x00);
	dac_expander_right.writePowerMode(0x00);
	dac_expander_right.writeGain(0x00);
}

void TouchSensorKit::calibrateTwoSensors(bool &sensor_left, bool &sensor_right, uint8_t channel)
{
	auto buffer_left  = std::array<uint8_t, 2> {};
	auto buffer_right = std::array<uint8_t, 2> {};

	dac_expander_left.writeToSpecificInputRegister(channel, buffer_left);
	dac_expander_right.writeToSpecificInputRegister(channel, buffer_right);
	rtos::ThisThread::sleep_for(1ms);

	auto value_left_calib  = uint16_t {0x0FFF};
	auto value_right_calib = uint16_t {0x0FFF};
	auto step			   = uint8_t {1};

	updateState();

	while (!((sensor_left || value_left_calib == 0) && (sensor_right || value_right_calib == 0))) {
		if (!sensor_left) {
			if (value_left_calib - step > 0x0FFF) {
				value_left_calib = 0x0FFF;
			} else {
				value_left_calib -= step;
			}
		}
		if (!sensor_right) {
			if (value_right_calib - step > 0x0FFF) {
				value_right_calib = 0x0FFF;
			} else {
				value_right_calib -= step;
			}
		}

		buffer_left.at(0) = static_cast<uint8_t>((0xFF00 & value_left_calib) >> 8);
		buffer_left.at(1) = static_cast<uint8_t>(0x00FF & value_left_calib);
		dac_expander_left.writeToSpecificInputRegister(channel, buffer_left);
		rtos::ThisThread::sleep_for(1ms);

		buffer_right.at(0) = static_cast<uint8_t>((0xFF00 & value_right_calib) >> 8);
		buffer_right.at(1) = static_cast<uint8_t>(0x00FF & value_right_calib);
		dac_expander_right.writeToSpecificInputRegister(channel, buffer_right);
		rtos::ThisThread::sleep_for(1ms);

		updateState();
	}

	dac_expander_left.writeToSpecificMemoryRegister(channel, buffer_left);
	dac_expander_right.writeToSpecificMemoryRegister(channel, buffer_right);
	rtos::ThisThread::sleep_for(1ms);

	log_info("CALIBRATED!");
	rtos::ThisThread::sleep_for(100ms);
}

void TouchSensorKit::calibrateEars()
{
	log_info("Place hands on EAR LEFT and EAR RIGHT");
	log_info("Calibration will start in 5 seconds");
	rtos::ThisThread::sleep_for(5s);
	calibrateTwoSensors(_ear_left_touched, _ear_right_touched, 2);
}

void TouchSensorKit::calibrateBeltLBRF()
{
	log_info("Place hands on BELT LEFT BACK and BELT RIGHT FRONT");
	log_info("Calibration will start in 5 seconds");
	rtos::ThisThread::sleep_for(5s);
	calibrateTwoSensors(_belt_left_back_touched, _belt_right_front_touched, 1);
}

void TouchSensorKit::calibrateBeltRBLF()
{
	log_info("Place hands on BELT LEFT FRONT and BELT RIGHT BACK");
	log_info("Calibration will start in 5 seconds");
	rtos::ThisThread::sleep_for(5s);
	calibrateTwoSensors(_belt_left_front_touched, _belt_right_back_touched, 0);
}

void TouchSensorKit::calibration()
{
	log_info("Touch calibration");
	log_info("For each of 6 touch sensors, value of sensibility will change");
	log_info("Please keep your hands on 2 sensors until \"CALIBRATED !\" appears.\n");
	rtos::ThisThread::sleep_for(15s);

	calibrateEars();
	calibrateBeltLBRF();
	calibrateBeltRBLF();
	rtos::ThisThread::sleep_for(1s);
}
