// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_
#define _LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_

#include <array>
#include <tuple>

#include "LKCoreI2C.h"
#include "LKUtils.h"
#include "hts221_reg.h"

namespace leka {

using celsius_t			 = float;
using relativeHumidity_t = float;

namespace interface {
	class LKCoreTemperatureSensor
	{
	  public:
		virtual void init()						 = 0;
		virtual celsius_t getTemperature()		 = 0;
		virtual relativeHumidity_t getHumidity() = 0;
	};
}	// namespace interface

class LKCoreTemperatureSensor : public interface::LKCoreTemperatureSensor
{
  public:
	explicit LKCoreTemperatureSensor(interface::I2C &i2c);

	typedef struct Calibration {
		utils::LinearFunctionValues humidity {0, 0};
		utils::LinearFunctionValues temperature {0, 0};
	} calibration_t;

	void init() final;
	celsius_t getTemperature() final;
	relativeHumidity_t getHumidity() final;
	void setCalibration(calibration_t sensor_calibration);
	const Calibration getCalibration();

  private:
	void calibrate();
	auto getReferenceTemperature();
	auto getReferenceHumidity();

	void powerOnDevice();
	void enableBlockDataUpdate();
	void setDataAquisitionRate();
	void powerOffHeater();
	void setNumberAverageTemperature();
	void setNumberAverageHumidity();

	float getReferenceTemperatureDegC0();
	float getReferenceTemperatureDegC1();
	float getReferenceTemperatureAdc0();
	float getReferenceTemperatureAdc1();

	float getReferenceHumidityRhO();
	float getReferenceHumidityRh1();
	float getReferenceHumidityAdc0();
	float getReferenceHumidityAdc1();

	int read(uint8_t register_address, uint8_t *pBuffer, uint16_t number_bytes_to_read);
	int write(uint8_t register_address, uint8_t *pBuffer, uint16_t number_bytes_to_write);

	stmdev_ctx_t _register_io_function;
	static int32_t ptr_io_write(void *handle, uint8_t register_address, uint8_t *p_buffer,
								uint16_t number_bytes_to_write);
	static int32_t ptr_io_read(void *handle, uint8_t register_address, uint8_t *p_buffer,
							   uint16_t number_bytes_to_read);

	const uint8_t _address = HTS221_I2C_ADDRESS;
	interface::I2C &_i2c;
	calibration_t _calibration;
	std::array<uint8_t, 32> _buffer {0};

	enum class State
	{
		OFF = 0,
		ON	= 1,
	};
};
}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_
