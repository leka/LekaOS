#include "LKCoreTemperatureSensor.h"

#include "gtest/gtest.h"
#include "stub_I2C.h"

using namespace leka;
mbed::I2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);

LKCoreTemperatureSensor temperatureSensor(i2c);

TEST(LKCoreTemperatureSensorTest, initialization)
{
	ASSERT_NE(&temperatureSensor, nullptr);
}

TEST(LKCoreTemperatureSensorTest, I2C_init)
{
	ASSERT_EQ(temperatureSensor.init(), Status::SUCCESS);
}

// TEST(LKCoreTemperatureSensorTest, initialization)
// {
// 	ASSERT_EQ(temperatureSensor.init(), Status::SUCCESS);
// }

// TEST(LKCoreTemperatureSensorTest, initialization)
// {
// 	ASSERT_NE(&temperatureSensor, nullptr);
// }
