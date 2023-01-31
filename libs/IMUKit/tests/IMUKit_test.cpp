// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "IMUKit.hpp"

#include "CoreLSM6DSOX.hpp"
#include "ThisThread.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"
#include "mocks/leka/LSM6DSOX.h"
#include "stubs/leka/EventLoopKit.h"
#include "stubs/mbed/InterruptIn.h"

using namespace leka;
using ::testing::_;
using ::testing::AtLeast;
using ::testing::SaveArg;

// TODO(@leka/dev-embedded): temporary fix, changes are needed when updating fusion algorithm

class IMUKitTest : public ::testing::Test
{
  protected:
	IMUKitTest() = default;

	void SetUp() override { imukit.init(); }
	// void TearDown() override {}

	mock::LSM6DSOX mock_lsm6dox {};

	IMUKit imukit {mock_lsm6dox};
};

TEST_F(IMUKitTest, initialization)
{
	ASSERT_NE(&imukit, nullptr);
}

TEST_F(IMUKitTest, start)
{
	EXPECT_CALL(mock_lsm6dox, setPowerMode(interface::LSM6DSOX::PowerMode::Normal)).Times(1);

	imukit.start();
}

TEST_F(IMUKitTest, stop)
{
	EXPECT_CALL(mock_lsm6dox, setPowerMode(interface::LSM6DSOX::PowerMode::Off)).Times(1);

	imukit.stop();
}

TEST_F(IMUKitTest, computeAnglesNullAccelerations)
{
	std::function<void(interface::LSM6DSOX::SensorData)> callback {};

	EXPECT_CALL(mock_lsm6dox, registerOnGyDataReadyCallback).WillOnce(SaveArg<0>(&callback));

	imukit.init();

	interface::LSM6DSOX::SensorData imu_data
	{
		{0, 0, 0}, { 0, 0, 0 }
	};

	callback(imu_data);

	auto [pitch, roll, yaw] = imukit.getAngles();

	for (auto i = 0; i < 100; ++i) {
		auto [pitch, roll, yaw] = imukit.getAngles();
		EXPECT_EQ(pitch, 0);
		EXPECT_EQ(roll, 0);
		EXPECT_EQ(yaw, 180);
	}
}

TEST_F(IMUKitTest, defaultPosition)
{
	std::function<void(interface::LSM6DSOX::SensorData)> callback {};

	EXPECT_CALL(mock_lsm6dox, registerOnGyDataReadyCallback).WillOnce(SaveArg<0>(&callback));

	imukit.init();

	interface::LSM6DSOX::SensorData imu_data
	{
		{0, 0, 1000}, { 0, 0, 0 }
	};

	callback(imu_data);

	auto [pitch, roll, yaw] = imukit.getAngles();

	for (auto i = 0; i < 100; ++i) {
		auto [pitch, roll, yaw] = imukit.getAngles();
		EXPECT_EQ(pitch, 0);
		EXPECT_EQ(roll, 0);
		EXPECT_EQ(yaw, 180);
	}
}

// TODO (@ladislas, @hugo) Go further with numeric testing when new fusion is implemented

// TEST_F(IMUKitTest, robotRolled90DegreesOnTheTop)
// {
// 	std::function<void(interface::LSM6DSOX::SensorData)> callback {};

// 	EXPECT_CALL(mock_lsm6dox, registerOnGyDataReadyCallback).WillOnce(SaveArg<0>(&callback));

// 	imukit.init();

// 	interface::LSM6DSOX::SensorData imu_data
// 	{
// 		{1000, 0, 0}, { 0, 0, 0 }
// 	};

// 	callback(imu_data);

// 	auto [pitch, roll, yaw] = imukit.getAngles();

// 	for (auto i = 0; i < 100; ++i) {
// 		auto [pitch, roll, yaw] = imukit.getAngles();
// 		EXPECT_TRUE(-100 < pitch && pitch < -80);
// 		EXPECT_EQ(roll, 0);
// 		EXPECT_EQ(yaw, 180);
// 	}
// }

// TEST_F(IMUKitTest, robotRolled90DegreesOnTheBottom)
// {
// 	std::function<void(interface::LSM6DSOX::SensorData)> callback {};

// 	EXPECT_CALL(mock_lsm6dox, registerOnGyDataReadyCallback).WillOnce(SaveArg<0>(&callback));

// 	imukit.init();

// 	interface::LSM6DSOX::SensorData imu_data
// 	{
// 		{-1000, 0, 0}, { 0, 0, 0 }
// 	};

// 	callback(imu_data);

// 	auto [pitch, roll, yaw] = imukit.getAngles();

// 	for (auto i = 0; i < 100; ++i) {
// 		auto [pitch, roll, yaw] = imukit.getAngles();
// 		EXPECT_TRUE(80 < pitch && pitch < 100);
// 		EXPECT_EQ(roll, 0);
// 		EXPECT_EQ(yaw, 180);
// 	}
// }

// TEST_F(IMUKitTest, robotRolled90DegreesOnItsLeft)
// {
// 	std::function<void(interface::LSM6DSOX::SensorData)> callback {};

// 	EXPECT_CALL(mock_lsm6dox, registerOnGyDataReadyCallback).WillOnce(SaveArg<0>(&callback));

// 	imukit.init();

// 	interface::LSM6DSOX::SensorData imu_data
// 	{
// 		{0, 1000, 0}, { 0, 0, 0 }
// 	};

// 	callback(imu_data);

// 	auto [pitch, roll, yaw] = imukit.getAngles();

// 	for (auto i = 0; i < 100; ++i) {
// 		auto [pitch, roll, yaw] = imukit.getAngles();
// 		EXPECT_EQ(pitch, 0);
// 		EXPECT_TRUE(80 < roll && roll < 100);
// 		EXPECT_EQ(yaw, 180);
// 	}
// }

// TEST_F(IMUKitTest, robotRolled90DegreesOnItsRight)
// {
// 	std::function<void(interface::LSM6DSOX::SensorData)> callback {};

// 	EXPECT_CALL(mock_lsm6dox, registerOnGyDataReadyCallback).WillOnce(SaveArg<0>(&callback));

// 	imukit.init();

// 	interface::LSM6DSOX::SensorData imu_data
// 	{
// 		{0, -1000, 0}, { 0, 0, 0 }
// 	};

// 	callback(imu_data);

// 	auto [pitch, roll, yaw] = imukit.getAngles();

// 	for (auto i = 0; i < 100; ++i) {
// 		auto [pitch, roll, yaw] = imukit.getAngles();
// 		EXPECT_EQ(pitch, 0);
// 		EXPECT_TRUE(-100 < roll && roll < -80);
// 		EXPECT_EQ(yaw, 180);
// 	}
// }
