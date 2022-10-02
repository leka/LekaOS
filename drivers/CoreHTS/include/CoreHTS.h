// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <tuple>

#include "CoreI2C.h"
#include "Utils.h"
#include "hts221_reg.h"

namespace leka {

namespace interface {

	class TemperatureSensor
	{
	  public:
		virtual ~TemperatureSensor() = default;

		virtual auto getTemperatureCelsius() -> float = 0;
	};

	class HumiditySensor
	{
	  public:
		virtual ~HumiditySensor() = default;

		virtual auto getRelativeHumidity() -> float = 0;
	};

}	// namespace interface

class CoreHTS : public interface::TemperatureSensor, public interface::HumiditySensor
{
	using Point				 = leka::utils::math::Point;
	using LinearCoefficients = leka::utils::math::LinearCoefficients;

  public:
	explicit CoreHTS(interface::I2C &i2c);

	void init();

	auto getTemperatureCelsius() -> float final;
	auto getRelativeHumidity() -> float final;

	void setLinearCalibrationForTemperature(LinearCoefficients coefficients);
	void setLinearCalibrationForHumidity(LinearCoefficients coefficients);

	[[nodiscard]] auto getTemperatureCalibrationCoefficients() const -> LinearCoefficients;
	[[nodiscard]] auto getHumidityCalibrationCoefficients() const -> LinearCoefficients;

	void turnOn();			// TODO: add turn off
	void turnOffHeater();	// TODO: add turn on?

	void enableBlockDataUpdate();
	void setDataAcquisitionRate();

	void setNumberOfMeasurementsToCalculateAverageTemperature();
	void setNumberOfMeasurementsToCalculateAverageHumidity();

	void calibrateTemperature();
	void calibrateHumidity();
	auto getCalibrationPointsForTemperature() -> std::tuple<Point, Point>;
	auto getCalibrationPointsForHumidity() -> std::tuple<Point, Point>;

  private:
	[[nodiscard]] auto generateCommandWithMultipleAccessToRegister(const uint8_t register_address) const -> uint8_t;

	auto read(uint8_t register_command, uint8_t *buffer, uint16_t size) -> int;
	auto write(uint8_t register_command, uint8_t *data, uint16_t size) -> int;

	stmdev_ctx_t _register_io_function;
	static auto ptr_io_write(void *handle, uint8_t register_address, uint8_t *p_buffer, uint16_t number_bytes_to_write)
		-> int32_t;
	static auto ptr_io_read(void *handle, uint8_t register_address, uint8_t *p_buffer, uint16_t number_bytes_to_read)
		-> int32_t;

	interface::I2C &_i2c;
	const uint8_t _I2C_ADDRESS = HTS221_I2C_ADDRESS;

	LinearCoefficients _temperature_coefficients;
	LinearCoefficients _humidity_coefficients;

	std::array<uint8_t, 32> _buffer {0};
};
}	// namespace leka
