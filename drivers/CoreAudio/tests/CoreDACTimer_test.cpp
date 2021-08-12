// // Leka - LekaOS
// // Copyright 2021 APF France handicap
// // SPDX-License-Identifier: Apache-2.0

#include "CoreDACTimer.h"

#include "gtest/gtest.h"
#include "mocks/leka/LKCoreSTM32Hal.h"

using namespace leka;
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::InSequence;
using ::testing::Return;

class CoreDACTimerTest : public ::testing::Test
{
  protected:
	CoreDACTimerTest() : coredactimer(halmock, CoreDACTimer::HardWareBasicTimer::BasicTimer6) {}

	LKCoreSTM32HalMock halmock;
	CoreDACTimer coredactimer;
};

TEST_F(CoreDACTimerTest, instantiation)
{
	ASSERT_NE(&coredactimer, nullptr);
}

TEST_F(CoreDACTimerTest, handleConfigurationInstance)
{
	auto handle = coredactimer.getHandle();

	ASSERT_EQ(handle.Instance, TIM6);
	// TODO() : make another test case with the TIM7
}

// TEST_F(CoreDACTimerTest, initialize)
// {
// 	float frequency = 44100;

// 	{
// 		InSequence seq;

// 		EXPECT_CALL(halmock, HAL_TIM_Base_Init).Times(1);
// 		EXPECT_CALL(halmock, HAL_TIMEx_MasterConfigSynchronization).Times(1);
// 	}
// 	coredactimer.initialize(frequency);
// 	auto handle = coredactimer.getHandle();

// 	ASSERT_EQ(handle.Init.Prescaler, 0);
// 	ASSERT_EQ(handle.Init.CounterMode, TIM_COUNTERMODE_UP);
// 	ASSERT_EQ(handle.Init.Period, 2448);   // TODO Handle different clock frequency cases
// 	ASSERT_EQ(handle.Init.AutoReloadPreload, TIM_AUTORELOAD_PRELOAD_DISABLE);
// }

TEST_F(CoreDACTimerTest, deInitialize)
{
	{
		EXPECT_CALL(halmock, HAL_TIM_Base_DeInit).Times(1);
	}
	coredactimer.terminate();
}

TEST_F(CoreDACTimerTest, start)
{
	{
		EXPECT_CALL(halmock, HAL_TIM_Base_Start).Times(1);
	}
	coredactimer.start();
}

TEST_F(CoreDACTimerTest, stop)
{
	{
		EXPECT_CALL(halmock, HAL_TIM_Base_Stop).Times(1);
	}
	coredactimer.stop();
}