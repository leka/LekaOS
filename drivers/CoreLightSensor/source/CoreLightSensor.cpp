#include "CoreLightSensor.h"

namespace leka {

auto CoreLightSensor::readRawValue() -> float
{
	return _pin.read();
}

auto CoreLightSensor::readLuminosity() -> float
{
	return static_cast<float>(1.0 - readRawValue());
}

auto CoreLightSensor::CoreLightSensor::getLuminosityLevel() -> LuminosityLevel
{
	auto luminosity = readLuminosity();

	constexpr auto luminosity_dark_value	= float {0.25};
	constexpr auto luminosity_shadow_value	= float {0.50};
	constexpr auto luminosity_ambient_value = float {0.75};

	if (luminosity <= luminosity_dark_value) {
		return LuminosityLevel::dark;
	}
	if (luminosity <= luminosity_shadow_value) {
		return LuminosityLevel::shadow;
	}
	if (luminosity <= luminosity_ambient_value) {
		return LuminosityLevel::ambient;
	}

	return LuminosityLevel::sunny;
}

}	// namespace leka
