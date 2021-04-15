// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_
#define _LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_

#include <array>

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
		virtual bool init()						 = 0;
		virtual celsius_t getTemperature()		 = 0;
		virtual relativeHumidity_t getHumidity() = 0;
	};
}	// namespace interface

class LKCoreTemperatureSensor : public interface::LKCoreTemperatureSensor
{
  public:
	explicit LKCoreTemperatureSensor(interface::LKCoreI2C &i2c);

	struct Calibration {
		utils::LinearInterpolationValues humidity;
		utils::LinearInterpolationValues temperature;
	};

	bool init() final;
	celsius_t getTemperature() final;
	relativeHumidity_t getHumidity() final;
	void setCalibration(float_t humidity_slope, float_t humidity_y_intercept, float_t temperature_slope,
						float_t temperature_y_intercept);
	const Calibration getCalibration();

  private:
	struct Reference;
	bool calibration();
	bool getReferenceTemperature(Reference &temperature_reference);
	bool getReferenceHumidity(Reference &humidity_reference);

	void temperatureCalibration(Reference const &temperature_reference);
	void humidityCalibration(Reference const &humidity_reference);

	bool setPower(uint8_t State);
	bool setBlock_data_update(uint8_t State);
	bool setDataAquisitionRate(hts221_odr_t rate);
	bool setHeater(uint8_t State);
	bool setAverageTemperature(hts221_avgt_t nbAvgTemp);
	bool setAverageHumidity(hts221_avgh_t nbAvgHum);

	int read(uint8_t register_address, uint8_t *pBuffer, uint16_t number_bytes_to_read);
	int write(uint8_t register_address, uint8_t *pBuffer, uint16_t number_bytes_to_write);

	stmdev_ctx_t _register_io_function;
	static int32_t ptr_io_write(void *handle, uint8_t register_address, uint8_t *p_buffer,
								uint16_t number_bytes_to_write);
	static int32_t ptr_io_read(void *handle, uint8_t register_address, uint8_t *p_buffer,
							   uint16_t number_bytes_to_read);

	const uint8_t _address = HTS221_I2C_ADDRESS;
	interface::LKCoreI2C &_i2c;
	Calibration _calibration;
	std::array<uint8_t, 32> _buffer = {0};

	struct Reference {
		float_t x0;
		float_t x1;
		float_t y0;
		float_t y1;
	};

	enum class State
	{
		OFF = 0,
		ON	= 1,
	};
};
}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_
