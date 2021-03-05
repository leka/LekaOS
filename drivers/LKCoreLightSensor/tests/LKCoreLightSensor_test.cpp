#include "LKCoreLightSensor.h"

#include "LKUtils.h"
#include "gtest/gtest.h"
#include "stub_AnalogIn.h"

using namespace leka;

LKCoreLightSensor sensor(PinName::SENSOR_LIGHT_ADC_INPUT);

float test_set_luminosity(float value)
{
	spy_AnalogIn_setValue(1 - value);	// the higher the luminosity, the lower the voltage
	return value;
}

TEST(LKCoreLuminositySensorTest, initialization)
{
	ASSERT_NE(&sensor, nullptr);
}

TEST(LKCoreLuminositySensorTest, readMinLuminosity)
{
	auto expected = test_set_luminosity(0.0f);

	ASSERT_EQ(expected, sensor.readLuminosity());
}

TEST(LKCoreLuminositySensorTest, readMiddleLuminosity)
{
	auto expected = test_set_luminosity(0.25f);

	ASSERT_EQ(expected, sensor.readLuminosity());
}

TEST(LKCoreLuminositySensorTest, readMaxLuminosity)
{
	auto expected = test_set_luminosity(1.0f);

	ASSERT_EQ(expected, sensor.readLuminosity());
}

TEST(LKCoreLuminosityTest, getLuminosityLevelDark)
{
	test_set_luminosity(0.10f);
	auto level = sensor.getLuminosityLevel();

	ASSERT_EQ(LKCoreLightSensor::LuminosityLevel::dark, level);
}

TEST(LKCoreLuminosityTest, getLuminosityLevelShadow)
{
	test_set_luminosity(0.34f);
	auto level = sensor.getLuminosityLevel();

	ASSERT_EQ(LKCoreLightSensor::LuminosityLevel::shadow, level);
}

TEST(LKCoreLuminosityTest, getLuminosityLevelAmbient)
{
	test_set_luminosity(0.68f);
	auto level = sensor.getLuminosityLevel();

	ASSERT_EQ(LKCoreLightSensor::LuminosityLevel::ambient, level);
}

TEST(LKCoreLuminosityTest, getLuminosityLevelSunny)
{
	test_set_luminosity(0.89f);
	auto level = sensor.getLuminosityLevel();

	ASSERT_EQ(LKCoreLightSensor::LuminosityLevel::sunny, level);
}
