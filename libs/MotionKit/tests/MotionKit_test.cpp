// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "MotionKit.h"

#include "IMUKit.h"
#include "gtest/gtest.h"
#include "mocks/leka/Accelerometer.h"
#include "mocks/leka/CoreMotor.h"
#include "mocks/leka/Gyroscope.h"
#include "mocks/leka/Timeout.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;

using ::testing::MockFunction;

ACTION_TEMPLATE(GetCallback, HAS_1_TEMPLATE_PARAMS(typename, callback_t), AND_1_VALUE_PARAMS(pointer))
{
	*pointer = callback_t(arg0);
}

class MotionKitTest : public ::testing::Test
{
  protected:
	MotionKitTest() = default;

	void SetUp() override
	{
		imukit.init();
		motion.init();
	}
	// void TearDown() override {}

	stub::EventLoopKit stub_event_loop_imu {};
	stub::EventLoopKit stub_event_loop_motion {};

	mock::CoreMotor mock_motor_left {};
	mock::CoreMotor mock_motor_right {};

	mock::Accelerometer accel {};
	mock::Gyroscope gyro {};

	mock::Timeout mock_timeout {};

	std::tuple<float, float, float> accel_data = {0.F, 0.F, 0.F};
	std::tuple<float, float, float> gyro_data  = {0.F, 0.F, 0.F};

	IMUKit imukit {stub_event_loop_imu, accel, gyro};

	MotionKit motion {mock_motor_left, mock_motor_right, imukit, stub_event_loop_motion, mock_timeout};
};

TEST_F(MotionKitTest, initialization)
{
	ASSERT_NE(&motion, nullptr);
}

TEST_F(MotionKitTest, registerMockCallbackAndRotate)
{
	auto mock_function_imu = MockFunction<void(void)> {};
	auto loop_imu		   = [&] { mock_function_imu.Call(); };

	auto mock_function_motion = MockFunction<void(void)> {};
	auto loop_motion		  = [&] { mock_function_motion.Call(); };

	EXPECT_CALL(mock_function_imu, Call()).Times(1);
	EXPECT_CALL(mock_function_motion, Call()).Times(1);

	EXPECT_CALL(mock_timeout, stop).Times(1);
	EXPECT_CALL(mock_motor_left, stop).Times(1);
	EXPECT_CALL(mock_motor_right, stop).Times(1);

	EXPECT_CALL(mock_timeout, onTimeout).Times(1);
	EXPECT_CALL(mock_timeout, start).Times(1);

	EXPECT_CALL(mock_motor_left, spin(Rotation::clockwise, 1)).Times(1);
	EXPECT_CALL(mock_motor_right, spin(Rotation::clockwise, 1)).Times(1);

	stub_event_loop_imu.registerCallback(loop_imu);
	stub_event_loop_motion.registerCallback(loop_motion);
	motion.rotate(1, Rotation::clockwise);
}

TEST_F(MotionKitTest, registerMockCallbackAndStartStabilisation)
{
	auto mock_function_imu = MockFunction<void(void)> {};
	auto loop_imu		   = [&] { mock_function_imu.Call(); };

	auto mock_function_motion = MockFunction<void(void)> {};
	auto loop_motion		  = [&] { mock_function_motion.Call(); };

	EXPECT_CALL(mock_function_imu, Call()).Times(1);
	EXPECT_CALL(mock_function_motion, Call()).Times(1);

	EXPECT_CALL(mock_timeout, stop).Times(1);
	EXPECT_CALL(mock_motor_left, stop).Times(1);
	EXPECT_CALL(mock_motor_right, stop).Times(1);

	stub_event_loop_imu.registerCallback(loop_imu);
	stub_event_loop_motion.registerCallback(loop_motion);
	motion.startStabilisation();
}

TEST_F(MotionKitTest, rotateAndStop)
{
	auto mock_function_imu = MockFunction<void(void)> {};
	auto loop_imu		   = [&] { mock_function_imu.Call(); };

	auto mock_function_motion = MockFunction<void(void)> {};
	auto loop_motion		  = [&] {
		 mock_function_motion.Call();
		 motion.stop();
	};

	EXPECT_CALL(mock_function_imu, Call()).Times(1);
	EXPECT_CALL(mock_function_motion, Call()).Times(1);

	EXPECT_CALL(mock_timeout, stop).Times(2);
	EXPECT_CALL(mock_motor_left, stop).Times(2);
	EXPECT_CALL(mock_motor_right, stop).Times(2);

	EXPECT_CALL(mock_motor_left, spin).Times(1);
	EXPECT_CALL(mock_motor_right, spin).Times(1);

	EXPECT_CALL(mock_timeout, onTimeout).Times(1);
	EXPECT_CALL(mock_timeout, start).Times(1);

	stub_event_loop_imu.registerCallback(loop_imu);
	stub_event_loop_motion.registerCallback(loop_motion);
	motion.rotate(1, Rotation::clockwise);
}

TEST_F(MotionKitTest, rotateAndTimeOutOver)
{
	auto mock_function_imu = MockFunction<void(void)> {};
	auto loop_imu		   = [&] { mock_function_imu.Call(); };

	auto mock_function_motion = MockFunction<void(void)> {};
	auto loop_motion		  = [&] { mock_function_motion.Call(); };

	interface::Timeout::callback_t on_timeout_callback = {};

	EXPECT_CALL(mock_function_imu, Call()).Times(1);
	EXPECT_CALL(mock_function_motion, Call()).Times(1);

	EXPECT_CALL(mock_timeout, stop).Times(1);
	EXPECT_CALL(mock_motor_left, stop).Times(1);
	EXPECT_CALL(mock_motor_right, stop).Times(1);

	EXPECT_CALL(mock_motor_left, spin).Times(1);
	EXPECT_CALL(mock_motor_right, spin).Times(1);

	EXPECT_CALL(mock_timeout, onTimeout).WillOnce(GetCallback<interface::Timeout::callback_t>(&on_timeout_callback));
	EXPECT_CALL(mock_timeout, start).Times(1);

	stub_event_loop_imu.registerCallback(loop_imu);
	stub_event_loop_motion.registerCallback(loop_motion);
	motion.rotate(1, Rotation::clockwise);

	EXPECT_CALL(mock_timeout, stop).Times(1);
	EXPECT_CALL(mock_motor_left, stop).Times(1);
	EXPECT_CALL(mock_motor_right, stop).Times(1);

	on_timeout_callback();
}

TEST_F(MotionKitTest, startStabilisationAndStop)
{
	auto mock_function_imu = MockFunction<void(void)> {};
	auto loop_imu		   = [&] { mock_function_imu.Call(); };

	auto mock_function_motion = MockFunction<void(void)> {};
	auto loop_motion		  = [&] {
		 mock_function_motion.Call();
		 motion.stop();
	};

	EXPECT_CALL(mock_function_imu, Call()).Times(1);
	EXPECT_CALL(mock_function_motion, Call()).Times(1);

	EXPECT_CALL(mock_timeout, stop).Times(2);
	EXPECT_CALL(mock_motor_left, stop).Times(2);
	EXPECT_CALL(mock_motor_right, stop).Times(2);

	stub_event_loop_imu.registerCallback(loop_imu);
	stub_event_loop_motion.registerCallback(loop_motion);
	motion.startStabilisation();
}
