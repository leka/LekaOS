#include "CoreLightSensor.h"

#include "gtest/gtest.h"
#include "stubs/mbed/AnalogIn.h"

using namespace leka;

CoreLightSensor sensor(PinName::SENSOR_LIGHT_ADC_INPUT);

float test_set_luminosity(float value)
{
	spy_AnalogIn_setValue(1 - value);	// the higher the luminosity, the lower the voltage
	return value;
}

TEST(CoreLuminositySensorTest, initialization)
{
	ASSERT_NE(&sensor, nullptr);
}

TEST(CoreLuminositySensorTest, readMinLuminosity)
{
	auto expected = test_set_luminosity(0.0f);

	ASSERT_EQ(expected, sensor.readLuminosity());
}

TEST(CoreLuminositySensorTest, readMiddleLuminosity)
{
	auto expected = test_set_luminosity(0.25f);

	ASSERT_EQ(expected, sensor.readLuminosity());
}

TEST(CoreLuminositySensorTest, readMaxLuminosity)
{
	auto expected = test_set_luminosity(1.0f);

	ASSERT_EQ(expected, sensor.readLuminosity());
}

TEST(CoreLuminosityTest, getLuminosityLevelDark)
{
	test_set_luminosity(0.10f);
	auto level = sensor.getLuminosityLevel();

	ASSERT_EQ(CoreLightSensor::LuminosityLevel::dark, level);
}

TEST(CoreLuminosityTest, getLuminosityLevelShadow)
{
	test_set_luminosity(0.34f);
	auto level = sensor.getLuminosityLevel();

	ASSERT_EQ(CoreLightSensor::LuminosityLevel::shadow, level);
}

TEST(CoreLuminosityTest, getLuminosityLevelAmbient)
{
	test_set_luminosity(0.68f);
	auto level = sensor.getLuminosityLevel();

	ASSERT_EQ(CoreLightSensor::LuminosityLevel::ambient, level);
}

TEST(CoreLuminosityTest, getLuminosityLevelSunny)
{
	test_set_luminosity(0.89f);
	auto level = sensor.getLuminosityLevel();

	ASSERT_EQ(CoreLightSensor::LuminosityLevel::sunny, level);
}
