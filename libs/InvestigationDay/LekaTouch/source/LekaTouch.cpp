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

#include "LogKit.h"

using namespace mbed;
using namespace std::chrono;

Touch::Touch()
	: _write_interface(SENSOR_PROXIMITY_MUX_I2C_SDA, SENSOR_PROXIMITY_MUX_I2C_SCL),
	  _read_interface(SENSOR_PROXIMITY_MUX_I2C_SDA, SENSOR_PROXIMITY_MUX_I2C_SCL, 0x4E),
	  _mux_reset(SENSOR_PROXIMITY_MUX_RESET, 0),
	  _mux_inta(SENSOR_PROXIMITY_MUX_IRQA),
	  _mux_intb(SENSOR_PROXIMITY_MUX_IRQB)
{
	/* Reset multiplexer before starting */

	_mux_reset = 1;

	initReadInterface();
	initWriteInterface(_write_address_left);
	initWriteInterface(_write_address_right);
}

void Touch::initReadInterface()
{
	/* Reset multiplexer (read interface) registers */
	_read_interface.reset();
	rtos::ThisThread::sleep_for(1ms);

	/* NB. for following functions, 2bytes are sent in order to set both bank (A and B) */

	/* Set direction of I/O of multiplexer - 0 = output (supply sensor), 1 = input (sensor data) */
	/* IODIRA register address 0x00 | IODIRB register address 0x01 (need pull-up) */
	_read_interface.inputOutputMask(0xFF00);   // IODIRB << 8 + IODIRA
	rtos::ThisThread::sleep_for(1ms);

	/* Set pull-up (value is 1), here to bank B */
	/* GPPUA register address 0x0C | GPPUB register addres 0x0D */
	_read_interface.internalPullupMask(0xFF00);	  // GPPUB << 8 + GPPUA
	rtos::ThisThread::sleep_for(1ms);

	/* Define interesting pins to be read, here 6 first GPIO/bits containing touch sensor return */
	/* GPIOA register address 0x12 | GPIOB register addres 0x13 */
	_read_interface.digitalWordWrite(0x003F);	// GPIOB << 8 + GPIOA
	rtos::ThisThread::sleep_for(1ms);

	return;
}

void Touch::initWriteInterface(uint8_t address)
{
	/* DAC (write interface) has 4 outputs. Due to the robot with its 6 sensors, 2 DACs are necessary. */
	/* There are two ways to write, one for configuration (presented here) and one for calibration */

	/* NB. In following comments, different way to write depend on 3 bits symbolized by C2, C1 and C0 */
	/* x refers to bit that does not count. In our configuration, they are set to 0 */

	/* Voltage reference, value set to VDD (0) instead of internal voltage (1) because it is higher */
	/* Structure of byte is : C2 C1 C0 x VrefA VrefB VrefC VrefD */
	const auto vref = std::array<char, 1> {0x80};
	_write_interface.write(address, vref.data(), 1);
	rtos::ThisThread::sleep_for(1ms);

	/* Power down, set to Normal mode(00) */
	/* Structure of 2-byte is : C2 C1 C0 x PD1A PD0A PD1B PD0B PD1C PD0C PD1D PD0D x x x x */
	const auto pd = std::array<char, 2> {0xA0, 0x00};
	_write_interface.write(address, pd.data(), 2);
	rtos::ThisThread::sleep_for(1ms);

	/* Gain, set to x1 (0) instead of x2 (1) */
	/* Structure of 2-byte is : C2 C1 C0 x GxA GxB GxC GxD */
	const auto gain = std::array<char, 1> {0xC0};
	_write_interface.write(address, gain.data(), 1);
	rtos::ThisThread::sleep_for(1ms);

	return;
}

void Touch::printAllReadInterfaceRegisters()
{
	for (uint16_t address = 0x00; address < 0x16; address += 0x01) {
		log_info("Register address %X -> %X", address, _read_interface.readRegister(address));
	}
}

void Touch::printAllWriteInterfaceRegisters(uint8_t address)
{
	/* Read DAC (write interface) is sequential, first register (3 bytes) then EEPROM (3 bytes) for each DAC */
	/* Structure is : CH_A_reg, CH_A_EEPROM, CH_B_reg, CH_B_EEPROM, CH_C_reg, CH_C_EEPROM, CH_D_reg, CH_D_EEPROM */
	const auto buffer_size = 24;   // 3 bytes * ((1 register + 1 EEPROM) * 4 channels) = 24 bytes
	auto buffer			   = std::array<char, buffer_size> {};

	_write_interface.read(address, buffer.data(), std::size(buffer));

	/* Refer to datasheet of MCP4728, figure 5-15 for interpretation */
	log_info("Read DAC (channel 0=A, 1=B, 2=C, 3=D):");
	for (uint8_t ch = 0; ch < 4; ch++) {
		log_info("Register of channel %d: %X %X %X", ch, buffer[ch * 6], buffer[ch * 6 + 1], buffer[ch * 6 + 2]);
		log_info("EEPROM of channel %d: %X %X %X", ch, buffer[ch * 6 + 3], buffer[ch * 6 + 4], buffer[ch * 6 + 5]);
	}

	return;
}

void Touch::calibrateTwoSensors(bool &sensor_left, bool &sensor_right, uint8_t channel)
{
	uint16_t value_left_calib  = 0x0FFF;
	uint16_t value_right_calib = 0x0FFF;
	uint16_t step			   = 0x0001;

	auto buffer = std::array<char, 3> {(uint8_t)(0x40 + (channel << 1 & 0x06)), 0x00, 0x00};
	buffer[0]	= (uint8_t)(0x40 + ((channel & 0x03) << 1));

	/* Reset calibration value to 0 on channel link to sensor */
	_write_interface.write(_write_address_left, buffer.data(), 3);
	_write_interface.write(_write_address_right, buffer.data(), 3);
	rtos::ThisThread::sleep_for(1s);
	updateSensorsStatus();

	while (!(sensor_left && sensor_right)) {
		/* Decrement values if hands are not detected */
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

		/* Multiple write mode and add channel information linked with sensor */
		/* Structure of multiple write is on 3 bytes, check datasheet for more information Figure 5-8 */
		buffer[1] = (uint8_t)((value_left_calib & 0x0F00) >> 8);
		buffer[2] = (uint8_t)((value_left_calib & 0x00FF) >> 0);
		_write_interface.write(_write_address_left, buffer.data(), 3);

		buffer[1] = (uint8_t)((value_right_calib & 0x0F00) >> 8);
		buffer[2] = (uint8_t)((value_right_calib & 0x00FF) >> 0);
		_write_interface.write(_write_address_right, buffer.data(), 3);

		/* Check sensors return */
		rtos::ThisThread::sleep_for(1ms);
		updateSensorsStatus();
	}

	/* Write in EEPROM values of calibration of touch sensor */
	/* Byte structure of write in EEPROM is -> C2 C1 C0 W1 W0 DAC1 DAC0 0 = 0 1 0 1 1 DAC1 DAC0 0 */
	buffer[0] = 0x58 + (channel << 1 & 0x06);
	buffer[1] = (uint8_t)(value_left_calib & 0x0F00 >> 8);
	buffer[2] = (uint8_t)(value_left_calib & 0x00FF >> 0);
	_write_interface.write(_write_address_left, buffer.data(), 3);

	buffer[1] = (uint8_t)(value_right_calib & 0x0F00 >> 8);
	buffer[2] = (uint8_t)(value_right_calib & 0x00FF >> 0);
	_write_interface.write(_write_address_right, buffer.data(), 3);
	rtos::ThisThread::sleep_for(1ms);

	log_info("CALIBRATED!");
	rtos::ThisThread::sleep_for(100ms);
	return;
}

void Touch::calibrateEars()
{
	log_info("Place hands on EAR LEFT and EAR RIGHT");
	log_info("Calibration will start in 5 seconds");
	rtos::ThisThread::sleep_for(5s);
	calibrateTwoSensors(_ear_left_touched, _ear_right_touched, 2);
}

void Touch::calibrateBeltLBRF()
{
	log_info("Place hands on BELT LEFT BACK and BELT RIGHT FRONT");
	log_info("Calibration will start in 5 seconds");
	rtos::ThisThread::sleep_for(5s);
	calibrateTwoSensors(_belt_left_back_touched, _belt_right_front_touched, 1);
}

void Touch::calibrateBeltRBLF()
{
	log_info("Place hands on BELT LEFT FRONT and BELT RIGHT BACK");
	log_info("Calibration will start in 5 seconds");
	rtos::ThisThread::sleep_for(5s);
	calibrateTwoSensors(_belt_left_front_touched, _belt_right_back_touched, 0);
}

void Touch::calibration()
{
	log_info("Touch calibration");
	log_info("For each of 6 touch sensors, value of sensibility will change");
	log_info("Please keep your hands on 2 sensors until \"CALIBRATED !\" appears.\n");
	rtos::ThisThread::sleep_for(15s);

	calibrateEars();
	calibrateBeltLBRF();
	calibrateBeltRBLF();

	return;
}

auto Touch::updateSensorsStatus() -> uint8_t
{
	auto value = (uint8_t)(~(_read_interface.digitalWordRead() >> 8));

	_ear_right_touched		  = (bool)((value >> 5) & 0x01);
	_ear_left_touched		  = (bool)((value >> 4) & 0x01);
	_belt_right_front_touched = (bool)((value >> 3) & 0x01);
	_belt_right_back_touched  = (bool)((value >> 2) & 0x01);
	_belt_left_back_touched	  = (bool)((value >> 1) & 0x01);
	_belt_left_front_touched  = (bool)((value >> 0) & 0x01);

	return value;
}

void Touch::start()
{
	log_info("Touch example");

	// printAllReadInterfaceRegisters();
	// printAllWriteInterfaceRegisters(_write_address_left);
	// printAllWriteInterfaceRegisters(_write_address_right);
	// calibration();

	int n_repetition = 10;
	while (true) {
		log_info("Start a cycle of %d checking of touch sensor every 1 second, then pause 10s", n_repetition);

		for (int i = 0; i < n_repetition; i++) {
			updateSensorsStatus();

			log_info("Ear left touched: %s", _ear_left_touched ? "true" : "false");
			log_info("Ear right touched: %s", _ear_right_touched ? "true" : "false");
			log_info("Belt left front touched: %s", _belt_left_front_touched ? "true" : "false");
			log_info("Belt left back touched: %s", _belt_left_back_touched ? "true" : "false");
			log_info("Belt right front touched: %s", _belt_right_front_touched ? "true" : "false");
			log_info("Belt right back touched: %s", _belt_right_back_touched ? "true" : "false");

			rtos::ThisThread::sleep_for(1s);
		}
		rtos::ThisThread::sleep_for(10s);
	}

	log_info("End of Touch example");
	return;
}
