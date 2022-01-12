#include "CoreLightSensor.h"

namespace leka {

float CoreLightSensor::readRawValue(void)
{
	return _pin.read();
}

float CoreLightSensor::readLuminosity(void)
{
	return (1.0 - readRawValue());
}

CoreLightSensor::LuminosityLevel CoreLightSensor::getLuminosityLevel()
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
