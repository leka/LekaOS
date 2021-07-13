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
	CoreDACTimerTest() : coredactimer(halmock) {}

	// void SetUp() override {}
	// void TearDown() override {}

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
}

// TODO FIX IT
// TEST_F(CoreDACTimerTest, initialize)
// {
// 	float frequency = 44100;
// 	auto handle		= coredactimer.getHandle();

// 	{
// 		InSequence seq;

// 		EXPECT_CALL(halmock, HAL_RCC_GetPCLK1Freq).Times(1);
// 		EXPECT_CALL(halmock, HAL_TIM_Base_Init).Times(1);
// 		EXPECT_CALL(halmock, HAL_TIMEx_MasterConfigSynchronization).Times(1);

// 		// Test the function called by the MSP, not sure if this
// 		EXPECT_CALL(halmock, HAL_RCC_TIM6_CLK_ENABLE).Times(1);
// 	}
// 	coredactimer.initialize(frequency);

// 	// Test init
// 	ASSERT_EQ(handle.Init.Prescaler, 0);
// 	ASSERT_EQ(handle.Init.CounterMode, TIM_COUNTERMODE_UP);
// 	ASSERT_EQ(handle.Init.Period, 2448);   // TODO Handle different clock frequency cases
// 	ASSERT_EQ(handle.Init.AutoReloadPreload, TIM_AUTORELOAD_PRELOAD_DISABLE);
// }

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

TEST_F(CoreDACTimerTest, deInitialize)
{
	{
		EXPECT_CALL(halmock, HAL_TIM_Base_DeInit).Times(1);
	}
	coredactimer.deInitialize();
}

TEST_F(CoreDACTimerTest, mspInitCallback)
{
	{
		EXPECT_CALL(halmock, HAL_RCC_TIM6_CLK_ENABLE).Times(1);
	}
	coredactimer._mspInitCallback();
}

TEST_F(CoreDACTimerTest, mspDeInitCallback)
{
	{
		EXPECT_CALL(halmock, HAL_RCC_TIM6_CLK_DISABLE).Times(1);
	}
	coredactimer._mspDeInitCallback();
}
