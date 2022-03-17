// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BehaviorKit.h"

#include "events/tests/UNITTESTS/doubles/EventQueue_stub.h"
#include "rtos/tests/UNITTESTS/doubles/Thread_stub.h"

#include "CoreMotor.h"
#include "CorePwm.h"
#include "CoreSPI.h"
#include "LedKit.h"
#include "VideoKit.h"
#include "gtest/gtest.h"
#include "mocks/leka/LEDAnimation.h"
#include "mocks/leka/PwmOut.h"
#include "mocks/mbed/DigitalOut.h"

using namespace leka;

class BehaviorKitTest : public ::testing::Test
{
  protected:
	BehaviorKitTest() : behaviorkit(videokit, ledkit, motor_left, motor_right) {};

	// void SetUp() override {}
	// void TearDown() override {}

	VideoKit videokit {};

	CoreSPI spi {NC, NC, NC, NC};
	CoreLED<LedKit::kNumberOfLedsBelt> belt {spi};
	CoreLED<LedKit::kNumberOfLedsEars> ears {spi};
	rtos::Thread animation_thread;
	events::EventQueue animation_event_queue;

	LedKit ledkit {animation_thread, animation_event_queue, ears, belt};

	mock::LEDAnimation mock_animation {};

	mbed::mock::DigitalOut dir_1_left = {};
	mbed::mock::DigitalOut dir_2_left = {};
	mock::PwmOut speed_left			  = {};

	mbed::mock::DigitalOut dir_1_right = {};
	mbed::mock::DigitalOut dir_2_right = {};
	mock::PwmOut speed_right		   = {};

	CoreMotor motor_left {dir_1_left, dir_2_left, speed_left};
	CoreMotor motor_right {dir_1_right, dir_2_right, speed_right};

	BehaviorKit behaviorkit;
};

TEST_F(BehaviorKitTest, initialization)
{
	ASSERT_NE(&behaviorkit, nullptr);
}

TEST_F(BehaviorKitTest, spinBlink)
{
	static constexpr auto speed = 0.5;

	EXPECT_CALL(dir_1_left, write(1));
	EXPECT_CALL(dir_2_left, write(0));
	EXPECT_CALL(speed_left, write(speed));
	EXPECT_CALL(dir_1_right, write(1));
	EXPECT_CALL(dir_2_right, write(0));
	EXPECT_CALL(speed_right, write(speed));

	behaviorkit.blinkGreen();
}

TEST_F(BehaviorKitTest, spinRight)
{
	static constexpr auto speed = 0.5;

	EXPECT_CALL(dir_1_left, write(0));
	EXPECT_CALL(dir_2_left, write(1));
	EXPECT_CALL(speed_left, write(speed));
	EXPECT_CALL(dir_1_right, write(0));
	EXPECT_CALL(dir_2_right, write(1));
	EXPECT_CALL(speed_right, write(speed));

	behaviorkit.spinBlink();
}

TEST_F(BehaviorKitTest, stop)
{
	static constexpr auto speed = 0.5;

	ledkit.start(&mock_animation);

	EXPECT_CALL(mock_animation, stop).Times(1);
	EXPECT_CALL(dir_1_left, write(0));
	EXPECT_CALL(dir_2_left, write(0));
	EXPECT_CALL(speed_left, write(0));
	EXPECT_CALL(dir_1_right, write(0));
	EXPECT_CALL(dir_2_right, write(0));
	EXPECT_CALL(speed_right, write(0));

	behaviorkit.stop();
}
