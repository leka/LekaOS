// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "IMUKit.h"

#include "ThisThread.h"
#include "gtest/gtest.h"
#include "mocks/leka/Accelerometer.h"
#include "mocks/leka/Gyroscope.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;

using ::testing::MockFunction;
using testing::Return;

class IMUKitTest : public ::testing::Test
{
  protected:
	IMUKitTest() = default;

	void SetUp() override { imukit.init(); }
	// void TearDown() override {}

	stub::EventLoopKit stub_event_loop {};

	mock::Accelerometer accel {};
	mock::Gyroscope gyro {};

	std::tuple<float, float, float> accel_data = {0.F, 0.F, 0.F};
	std::tuple<float, float, float> gyro_data  = {0.F, 0.F, 0.F};

	IMUKit imukit {stub_event_loop, accel, gyro};
};

TEST_F(IMUKitTest, initialization)
{
	ASSERT_NE(&imukit, nullptr);
}

TEST_F(IMUKitTest, registerMockCallbackAndStart)
{
	auto mock_function = MockFunction<void(void)> {};
	auto loop		   = [&] { mock_function.Call(); };

	EXPECT_CALL(mock_function, Call()).Times(1);

	stub_event_loop.registerCallback(loop);
	imukit.start();
}

TEST_F(IMUKitTest, start10Loops)
{
	auto mock_function = MockFunction<void(void)> {};
	auto loop		   = [&] { mock_function.Call(); };

	stub_event_loop.spy_setNumberOfLoops(10);
	EXPECT_CALL(mock_function, Call()).Times(10);

	stub_event_loop.registerCallback(loop);
	imukit.start();
}

TEST_F(IMUKitTest, stop)
{
	auto mock_function = MockFunction<void(void)> {};
	auto loop		   = [&] {
		 mock_function.Call();
		 imukit.stop();
	};

	stub_event_loop.spy_setNumberOfLoops(2);
	EXPECT_CALL(mock_function, Call()).Times(1);

	stub_event_loop.registerCallback(loop);
	imukit.start();
}

TEST_F(IMUKitTest, run)
{
	auto mock_function = MockFunction<void(void)> {};
	auto loop		   = [&] {
		 mock_function.Call();
		 imukit.stop();
		 imukit.run();
	};

	stub_event_loop.spy_setNumberOfLoops(2);
	EXPECT_CALL(mock_function, Call()).Times(1);

	stub_event_loop.registerCallback(loop);
	imukit.start();
}

TEST_F(IMUKitTest, computeAnglesNullAccelerations)
{
	accel_data = {0.F, 0.F, 0.F};
	gyro_data  = {0.F, 0.F, 0.F};

	EXPECT_CALL(accel, getXYZ).WillRepeatedly(Return(accel_data));
	EXPECT_CALL(gyro, getXYZ).WillRepeatedly(Return(gyro_data));

	auto [pitch, roll, yaw] = imukit.getAngles();

	for (auto i = 0; i < 100; ++i) {
		imukit.computeAngles();

		auto [pitch, roll, yaw] = imukit.getAngles();
		EXPECT_EQ(pitch, 0);
		EXPECT_EQ(roll, 0);
		EXPECT_EQ(yaw, 180);
	}
}

TEST_F(IMUKitTest, defaultPosition)
{
	accel_data = {0.F, 0.F, 1000.F};

	EXPECT_CALL(accel, getXYZ).WillRepeatedly(Return(accel_data));

	for (auto i = 0; i < 100; ++i) {
		imukit.computeAngles();
	}

	auto [pitch, roll, yaw] = imukit.getAngles();

	EXPECT_EQ(pitch, 0);
	EXPECT_EQ(roll, 0);
	EXPECT_EQ(yaw, 180);
}

TEST_F(IMUKitTest, robotRolled90DegreesOnTheTop)
{
	accel_data = {1000.F, 0.F, 0.F};

	EXPECT_CALL(accel, getXYZ).WillRepeatedly(Return(accel_data));

	for (auto i = 0; i < 100; ++i) {
		imukit.computeAngles();
	}

	auto [pitch, roll, yaw] = imukit.getAngles();

	EXPECT_TRUE(-100 < pitch && pitch < -80);
	EXPECT_EQ(roll, 0);
	EXPECT_EQ(yaw, 180);
}

TEST_F(IMUKitTest, robotRolled90DegreesOnTheBottom)
{
	accel_data = {-1000.F, 0.F, 0.F};

	EXPECT_CALL(accel, getXYZ).WillRepeatedly(Return(accel_data));

	for (auto i = 0; i < 100; ++i) {
		imukit.computeAngles();
	}

	auto [pitch, roll, yaw] = imukit.getAngles();

	EXPECT_TRUE(80 < pitch && pitch < 100);
	EXPECT_EQ(roll, 0);
	EXPECT_EQ(yaw, 180);
}

TEST_F(IMUKitTest, robotRolled90DegreesOnItsLeft)
{
	accel_data = {0.F, 1000.F, 0.F};

	EXPECT_CALL(accel, getXYZ).WillRepeatedly(Return(accel_data));

	for (auto i = 0; i < 100; ++i) {
		imukit.computeAngles();
	}

	auto [pitch, roll, yaw] = imukit.getAngles();

	EXPECT_EQ(pitch, 0);
	EXPECT_TRUE(80 < roll && roll < 100);
	EXPECT_EQ(yaw, 180);
}

TEST_F(IMUKitTest, robotRolled90DegreesOnItsRight)
{
	accel_data = {0.F, -1000.F, 0.F};

	EXPECT_CALL(accel, getXYZ).WillRepeatedly(Return(accel_data));

	for (auto i = 0; i < 100; ++i) {
		imukit.computeAngles();
	}

	auto [pitch, roll, yaw] = imukit.getAngles();

	EXPECT_EQ(pitch, 0);
	EXPECT_TRUE(-100 < roll && roll < -80);
	EXPECT_EQ(yaw, 180);
}

TEST_F(IMUKitTest, defaultPositionWithAngularSpeed50dps)
{
	accel_data = {0.F, 0.F, 1000.F};
	gyro_data  = {0.F, 0.F, 50.F};

	EXPECT_CALL(accel, getXYZ).WillRepeatedly(Return(accel_data));
	EXPECT_CALL(gyro, getXYZ).WillRepeatedly(Return(gyro_data));

	for (auto i = 0; i < 100; ++i) {
		imukit.computeAngles();
	}

	auto [pitch, roll, yaw] = imukit.getAngles();

	EXPECT_EQ(pitch, 0);
	EXPECT_EQ(roll, 0);
	EXPECT_TRUE(210 < yaw && yaw < 230);
}

TEST_F(IMUKitTest, defaultPositionWithAngularSpeedMinus50dps)
{
	accel_data = {0.F, 0.F, 1000.F};
	gyro_data  = {0.F, 0.F, -50.F};

	EXPECT_CALL(accel, getXYZ).WillRepeatedly(Return(accel_data));
	EXPECT_CALL(gyro, getXYZ).WillRepeatedly(Return(gyro_data));

	for (auto i = 0; i < 100; ++i) {
		imukit.computeAngles();
	}

	auto [pitch, roll, yaw] = imukit.getAngles();

	EXPECT_EQ(pitch, 0);
	EXPECT_EQ(roll, 0);
	EXPECT_TRUE(130 < yaw && yaw < 150);
}

TEST_F(IMUKitTest, defaultPositionWithAngularSpeed50dpsThenReset)
{
	accel_data = {0.F, 0.F, 1000.F};
	gyro_data  = {0.F, 0.F, 50.F};

	EXPECT_CALL(accel, getXYZ).WillRepeatedly(Return(accel_data));
	EXPECT_CALL(gyro, getXYZ).WillRepeatedly(Return(gyro_data));

	for (auto i = 0; i < 100; ++i) {
		imukit.computeAngles();
	}

	auto [pitch, roll, yaw] = imukit.getAngles();

	EXPECT_TRUE(210 < yaw && yaw < 230);

	imukit.reset();

	imukit.computeAngles();

	auto [after_reset_pitch, after_reset_roll, after_reset_yaw] = imukit.getAngles();

	EXPECT_TRUE(180 <= after_reset_yaw && after_reset_yaw < 190);
}
