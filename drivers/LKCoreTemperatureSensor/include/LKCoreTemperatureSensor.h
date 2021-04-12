// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_
#define _LEKA_OS_DRIVER_LK_CORE_HTS221_DRIVER_H_

#include <array>

#include "LKCoreI2C.h"
#include "LKCoreTemperatureSensorBase.h"
#include "hts221_reg.h"
#include "status.h"

namespace leka {

namespace state {
	constexpr uint8_t ON  = 1;
	constexpr uint8_t OFF = 0;
};	 // namespace state

struct calibrationValues {
	bool initialisation = 0;
	float_t humiditySlope {0};
	float_t humidity_y_intercept {0};
	float_t temperatureSlope {0};
	float_t temperature_y_intercept {0};
};

class LKCoreTemperatureSensor : public LKCoreTemperatureSensorDriverBase
{
  public:
	explicit LKCoreTemperatureSensor(interface::LKCoreI2C &i2c);
	virtual ~LKCoreTemperatureSensor() = default;

	virtual bool init();
	virtual bool calibration();
	virtual calibrationValues getCalibrationValues();
	virtual celsius_t getTemperature();
	virtual rH_t getHumidity();

  private:
	interface::LKCoreI2C &_i2c;
	calibrationValues _calibration;
	uint8_t _address				= HTS221_I2C_ADDRESS;
	std::array<uint8_t, 32> _buffer = {0};

  private:
	bool setPower(uint8_t);
	bool setBDU(uint8_t);
	bool setDataAquisitionRate(hts221_odr_t);
	bool setHeater(uint8_t);
	bool setAvgTemperature(hts221_avgt_t);
	bool setAvgHumidity(hts221_avgh_t);

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
