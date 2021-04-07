// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_
#define _LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_

#include <array>

#include "I2C.h"
#include "hts221_reg.h"
#include "status.h"

namespace leka {

namespace state {
	constexpr uint8_t ON  = 1;
	constexpr uint8_t OFF = 0;
};	 // namespace state

class LKCoreHTS221Driver
{
  public:
	explicit LKCoreHTS221Driver(mbed::I2C &i2c);
	virtual status_t init();
	virtual status_t calibration();
	virtual uint8_t getId();
	virtual int16_t getRawTemperature();
	virtual int16_t getRawHumidity();

  private:
	mbed::I2C &_i2c;
	float_t _humiditySlope {0};
	float_t _humidity_y_intercept {0};
	float_t _temperatureSlope {0};
	float_t _temperature_y_intercept {0};

	uint8_t _address				= HTS221_I2C_ADDRESS;
	std::array<uint8_t, 32> _buffer = {0};

  private:
	int read(uint8_t register_address, uint8_t *pBuffer, uint16_t number_bytes_to_read);
	int write(uint8_t register_address, uint8_t *pBuffer, uint16_t number_bytes_to_write);

	stmdev_ctx_t _register_io_function;
	static int32_t ptr_io_write(void *handle, uint8_t register_address, uint8_t *p_buffer,
								uint16_t number_bytes_to_write);
	static int32_t ptr_io_read(void *handle, uint8_t register_address, uint8_t *p_buffer,
							   uint16_t number_bytes_to_read);
};
}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_
