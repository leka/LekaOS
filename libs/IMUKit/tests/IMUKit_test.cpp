// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "IMUKit.hpp"

#include "CoreLSM6DSOX.hpp"
#include "ThisThread.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"
#include "mocks/leka/IMU.h"
#include "stubs/leka/EventLoopKit.h"
#include "stubs/mbed/InterruptIn.h"
#include "stubs/mbed/Kernel.h"

using namespace leka;
using namespace std::chrono;

class IMUKitTest : public ::testing::Test
{
  protected:
	IMUKitTest() = default;

	void SetUp() override { imukit.init(); }
	// void TearDown() override {}

	mock::IMU mock_imu {};

	IMUKit imukit {mock_imu};
};

TEST_F(IMUKitTest, initialization)
{
	ASSERT_NE(&imukit, nullptr);
}

TEST_F(IMUKitTest, start)
{
	EXPECT_CALL(mock_imu, setPowerMode(interface::IMU::PowerMode::Normal)).Times(1);

	imukit.start();
}

TEST_F(IMUKitTest, stop)
{
	EXPECT_CALL(mock_imu, setPowerMode(interface::IMU::PowerMode::Off)).Times(1);

	imukit.stop();
}

TEST_F(IMUKitTest, getEulerAngles)
{
	auto [pitch, roll, yaw] = imukit.getEulerAngles();

	EXPECT_EQ(pitch, 0);
	EXPECT_EQ(roll, 0);
	EXPECT_EQ(yaw, 0);
}

TEST_F(IMUKitTest, setOrigin)
{
	// TODO(@ladislas): add tests
	imukit.setOrigin();
}

TEST_F(IMUKitTest, onDataReady)
{
	testing::MockFunction<void(EulerAngles angles)> mock_callback {};

	imukit.onEulerAnglesReady(mock_callback.AsStdFunction());

	const auto data_initial =
		interface::IMU::SensorData {.xl = {.x = 0.F, .y = 0.F, .z = 0.F}, .gy = {.x = 0.F, .y = 0.F, .z = 0.F}};

	EXPECT_CALL(mock_callback, Call);

	mock_imu.call_drdy_callback(data_initial);

	const auto angles_initial = imukit.getEulerAngles();

	spy_kernel_addElapsedTimeToTickCount(80ms);

	const auto data_updated =
		interface::IMU::SensorData {.xl = {.x = 1.F, .y = 2.F, .z = 3.F}, .gy = {.x = 1.F, .y = 2.F, .z = 3.F}};

	EXPECT_CALL(mock_callback, Call);

	mock_imu.call_drdy_callback(data_updated);

	auto angles_updated = imukit.getEulerAngles();

	EXPECT_NE(angles_initial.pitch, angles_updated.pitch);
	EXPECT_NE(angles_initial.roll, angles_updated.roll);
	EXPECT_NE(angles_initial.yaw, angles_updated.yaw);
}

TEST_F(IMUKitTest, onDataReadyEmptyEulerAngleCallback)
{
	imukit.onEulerAnglesReady({});

	const auto data_initial =
		interface::IMU::SensorData {.xl = {.x = 0.F, .y = 0.F, .z = 0.F}, .gy = {.x = 0.F, .y = 0.F, .z = 0.F}};

	mock_imu.call_drdy_callback(data_initial);

	const auto angles_initial = imukit.getEulerAngles();

	spy_kernel_addElapsedTimeToTickCount(80ms);

	const auto data_updated =
		interface::IMU::SensorData {.xl = {.x = 1.F, .y = 2.F, .z = 3.F}, .gy = {.x = 1.F, .y = 2.F, .z = 3.F}};

	mock_imu.call_drdy_callback(data_updated);

	auto angles_updated = imukit.getEulerAngles();

	EXPECT_NE(angles_initial.pitch, angles_updated.pitch);
	EXPECT_NE(angles_initial.roll, angles_updated.roll);
	EXPECT_NE(angles_initial.yaw, angles_updated.yaw);
}
