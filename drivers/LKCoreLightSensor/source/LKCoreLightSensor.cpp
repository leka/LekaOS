#include "LKCoreLightSensor.h"

namespace leka {

float LKCoreLightSensor::readRawValue(void)
{
	return _pin.read();
}

float LKCoreLightSensor::readLuminosity(void)
{
	return (1.0 - readRawValue());
}

LKCoreLightSensor::LuminosityLevel LKCoreLightSensor::getLuminosityLevel()
{
	auto luminosity = readLuminosity();

	if (luminosity <= 0.25f) {
		return LuminosityLevel::dark;
	}
	if (luminosity <= 0.50f) {
		return LuminosityLevel::shadow;
	}
	if (luminosity <= 0.75f) {
		return LuminosityLevel::ambient;
	}

	return LuminosityLevel::sunny;
}

}	// namespace leka
