// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_
#define _LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_

#include <array>

#include "LKCoreI2C.h"
#include "LKCoreTemperatureSensorBase.h"
#include "hts221_reg.h"

namespace leka {

class LKCoreTemperatureSensor : public LKCoreTemperatureSensorDriverBase
{
  public:
	explicit LKCoreTemperatureSensor(interface::LKCoreI2C &i2c);

	struct CalibrationValues {
		float_t slope {0};
		float_t y_intercept {0};
	};

	struct Calibration {
		bool is_initialise {false};
		CalibrationValues humidity;
		CalibrationValues temperature;
	};

	bool init() final;
	celsius_t getTemperature() final;
	virtualHumidity_t getHumidity() final;
	const Calibration getCalibration();

  private:
	bool calibration();
	bool setPower(uint8_t State);
	bool setBlock_data_update(uint8_t State);
	bool setDataAquisitionRate(hts221_odr_t rate);
	bool setHeater(uint8_t State);
	bool setAvgTemperature(hts221_avgt_t nbAvgTemp);
	bool setAvgHumidity(hts221_avgh_t nbAvgHum);

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

	enum class State
	{
		OFF = 0,
		ON	= 1,
	};
};
}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_
