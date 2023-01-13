// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "TouchSensorKit.h"

#include "gtest/gtest.h"
#include "mocks/leka/CoreTouchSensor.h"
#include "mocks/leka/EventQueue.h"

using namespace leka;

using ::testing::MockFunction;
using ::testing::Return;

class TouchSensorKitTest : public ::testing::Test
{
  protected:
	TouchSensorKitTest() = default;

	void SetUp() override
	{
		expectedCallsInitialize();
		touch_sensor_kit.initialize();
	}
	// void TearDown() override {}

	mock::CoreTouchSensor mock_ear_left;
	mock::CoreTouchSensor mock_ear_right;
	mock::CoreTouchSensor mock_belt_left_back;
	mock::CoreTouchSensor mock_belt_left_front;
	mock::CoreTouchSensor mock_belt_right_back;
	mock::CoreTouchSensor mock_belt_right_front;

	mock::EventQueue event_queue {};

	TouchSensorKit touch_sensor_kit {mock_ear_left,		   mock_ear_right,		 mock_belt_left_back,
									 mock_belt_left_front, mock_belt_right_back, mock_belt_right_front};

	void expectedCallsInitialize()
	{
		EXPECT_CALL(mock_ear_left, init).Times(1);
		EXPECT_CALL(mock_ear_right, init).Times(1);
		EXPECT_CALL(mock_belt_left_front, init).Times(1);
		EXPECT_CALL(mock_belt_left_back, init).Times(1);
		EXPECT_CALL(mock_belt_right_front, init).Times(1);
		EXPECT_CALL(mock_belt_right_back, init).Times(1);

		EXPECT_CALL(mock_ear_left, setSensitivity).Times(1);
		EXPECT_CALL(mock_ear_right, setSensitivity).Times(1);
		EXPECT_CALL(mock_belt_left_front, setSensitivity).Times(1);
		EXPECT_CALL(mock_belt_left_back, setSensitivity).Times(1);
		EXPECT_CALL(mock_belt_right_front, setSensitivity).Times(1);
		EXPECT_CALL(mock_belt_right_back, setSensitivity).Times(1);
	}
};

TEST_F(TouchSensorKitTest, initializationDefault)
{
	ASSERT_NE(&touch_sensor_kit, nullptr);
}

TEST_F(TouchSensorKitTest, initialize)
{
	EXPECT_CALL(mock_ear_left, init).Times(1);
	EXPECT_CALL(mock_ear_right, init).Times(1);
	EXPECT_CALL(mock_belt_left_front, init).Times(1);
	EXPECT_CALL(mock_belt_left_back, init).Times(1);
	EXPECT_CALL(mock_belt_right_front, init).Times(1);
	EXPECT_CALL(mock_belt_right_back, init).Times(1);

	EXPECT_CALL(mock_ear_left, setSensitivity).Times(1);
	EXPECT_CALL(mock_ear_right, setSensitivity).Times(1);
	EXPECT_CALL(mock_belt_left_front, setSensitivity).Times(1);
	EXPECT_CALL(mock_belt_left_back, setSensitivity).Times(1);
	EXPECT_CALL(mock_belt_right_front, setSensitivity).Times(1);
	EXPECT_CALL(mock_belt_right_back, setSensitivity).Times(1);

	touch_sensor_kit.initialize();
}

TEST_F(TouchSensorKitTest, setRefreshDelay)
{
	touch_sensor_kit.setRefreshDelay(std::chrono::milliseconds {100});

	// nothing expected
}

TEST_F(TouchSensorKitTest, enable)
{
	EXPECT_CALL(mock_ear_left, read).Times(1);
	EXPECT_CALL(mock_ear_right, read).Times(1);
	EXPECT_CALL(mock_belt_left_front, read).Times(1);
	EXPECT_CALL(mock_belt_left_back, read).Times(1);
	EXPECT_CALL(mock_belt_right_front, read).Times(1);
	EXPECT_CALL(mock_belt_right_back, read).Times(1);

	touch_sensor_kit.enable();
}

TEST_F(TouchSensorKitTest, disable)
{
	touch_sensor_kit.disable();

	// nothing expected
}

TEST_F(TouchSensorKitTest, registerOnSensorTouched)
{
	MockFunction<void(const Position)> mock_callback;
	touch_sensor_kit.registerOnSensorTouched(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_ear_left, read).WillOnce(Return(false));
	EXPECT_CALL(mock_ear_right, read).WillOnce(Return(false));
	EXPECT_CALL(mock_belt_left_front, read).WillOnce(Return(false));
	EXPECT_CALL(mock_belt_left_back, read).WillOnce(Return(false));
	EXPECT_CALL(mock_belt_right_front, read).WillOnce(Return(false));
	EXPECT_CALL(mock_belt_right_back, read).WillOnce(Return(false));

	touch_sensor_kit.enable();

	EXPECT_CALL(mock_ear_left, read).WillOnce(Return(true));
	EXPECT_CALL(mock_callback, Call(Position::ear_left)).Times(1);

	EXPECT_CALL(mock_ear_right, read).WillOnce(Return(false));
	EXPECT_CALL(mock_callback, Call(Position::ear_right)).Times(0);

	EXPECT_CALL(mock_belt_left_front, read).WillOnce(Return(true));
	EXPECT_CALL(mock_callback, Call(Position::belt_left_front)).Times(1);

	EXPECT_CALL(mock_belt_left_back, read).WillOnce(Return(false));
	EXPECT_CALL(mock_callback, Call(Position::belt_left_back)).Times(0);

	EXPECT_CALL(mock_belt_right_front, read).WillOnce(Return(true));
	EXPECT_CALL(mock_callback, Call(Position::belt_right_front)).Times(1);

	EXPECT_CALL(mock_belt_right_back, read).WillOnce(Return(false));
	EXPECT_CALL(mock_callback, Call(Position::belt_right_back)).Times(0);

	touch_sensor_kit.enable();
}

TEST_F(TouchSensorKitTest, registerOnSensorReleased)
{
	MockFunction<void(const Position)> mock_callback;
	touch_sensor_kit.registerOnSensorReleased(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_ear_left, read).WillOnce(Return(true));
	EXPECT_CALL(mock_ear_right, read).WillOnce(Return(true));
	EXPECT_CALL(mock_belt_left_front, read).WillOnce(Return(true));
	EXPECT_CALL(mock_belt_left_back, read).WillOnce(Return(true));
	EXPECT_CALL(mock_belt_right_front, read).WillOnce(Return(true));
	EXPECT_CALL(mock_belt_right_back, read).WillOnce(Return(true));

	touch_sensor_kit.enable();

	EXPECT_CALL(mock_ear_left, read).WillOnce(Return(true));
	EXPECT_CALL(mock_callback, Call(Position::ear_left)).Times(0);

	EXPECT_CALL(mock_ear_right, read).WillOnce(Return(true));
	EXPECT_CALL(mock_callback, Call(Position::ear_right)).Times(0);

	EXPECT_CALL(mock_belt_left_front, read).WillOnce(Return(false));
	EXPECT_CALL(mock_callback, Call(Position::belt_left_front)).Times(1);

	EXPECT_CALL(mock_belt_left_back, read).WillOnce(Return(true));
	EXPECT_CALL(mock_callback, Call(Position::belt_left_back)).Times(0);

	EXPECT_CALL(mock_belt_right_front, read).WillOnce(Return(false));
	EXPECT_CALL(mock_callback, Call(Position::belt_right_front)).Times(1);

	EXPECT_CALL(mock_belt_right_back, read).WillOnce(Return(false));
	EXPECT_CALL(mock_callback, Call(Position::belt_right_back)).Times(1);

	touch_sensor_kit.enable();
}
