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

	// TODO: These EXPECT_CALL suppress the GMOCK WARNING: Uninteresting mock function call
	// TODO: Remove them in the future
	void silenceUnexpectedCalls(void)
	{
		EXPECT_CALL(halmock, HAL_RCC_DMA2_CLK_ENABLE).Times(AnyNumber());
		EXPECT_CALL(halmock, HAL_RCC_GPIOD_CLK_ENABLE).Times(AnyNumber());
		EXPECT_CALL(halmock, HAL_RCC_GPIOE_CLK_ENABLE).Times(AnyNumber());
		EXPECT_CALL(halmock, HAL_RCC_GPIOF_CLK_ENABLE).Times(AnyNumber());
		EXPECT_CALL(halmock, HAL_RCC_GPIOG_CLK_ENABLE).Times(AnyNumber());
		EXPECT_CALL(halmock, HAL_RCC_GPIOH_CLK_ENABLE).Times(AnyNumber());
		EXPECT_CALL(halmock, HAL_RCC_GPIOI_CLK_ENABLE).Times(AnyNumber());
		EXPECT_CALL(halmock, HAL_GPIO_Init).Times(AnyNumber());
		EXPECT_CALL(halmock, HAL_LINKDMA).Times(AnyNumber());
		EXPECT_CALL(halmock, HAL_DMA_DeInit).Times(AnyNumber());
		EXPECT_CALL(halmock, HAL_DMA_Init).Times(AnyNumber());
		EXPECT_CALL(halmock, HAL_NVIC_SetPriority).Times(AnyNumber());
		EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ).Times(AnyNumber());
		EXPECT_CALL(halmock, HAL_SDRAM_SendCommand).Times(AnyNumber());
	};
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

TEST_F(CoreDACTimerTest, initialize)
{
	float frequency = 100;
	auto handle = coredactimer.getHandle();

	{
		InSequence seq;

		EXPECT_CALL(coredactimer, _registerMspCallbacks).Times(1);
		EXPECT_CALL(halmock,HAL_TIM_Base_Init).Times(1);
		EXPECT_CALL(halmock,HAL_TIMEx_MasterConfigSynchronization).Times(1);
	}

	coredactimer.initialize(frequency);

	ASSERT_EQ(handle.Init.Prescaler, 0);
	ASSERT_EQ(handle.Init.CounterMode, TIM_COUNTERMODE_UP);
	ASSERT_EQ(handle.Init.Period, _calculatePeriod(frequency));
	ASSERT_EQ(handle.Init.AutoReloadPreload, TIM_AUTORELOAD_PRELOAD_DISABLE);
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

TEST_F(CoreDACTimerTest, deInitialize)
{
	{
		EXPECT_CALL(halmock, HAL_TIM_Base_DeInit).Times(1);
	}
	coredactimer.deInitialize();
}

TEST_F(CoreDACTimerTest, _calculatePeriod)
{
	float frequency = 44100;
	ASSERT_EQ(coredactimer._calculatePeriod(frequency), 2448);
}

TEST_F(CoreDACTimerTest, _registerMspCallbacks)
{
	{
		EXPECT_CALL(halmock,HAL_TIM_RegisterCallback).Times(2);
	}
	coredactimer._registerMspCallbacks();
}

TEST_F(CoreDACTimerTest, _mspInitCallback)
{
	{
		EXPECT_CALL(halmock,HAL_RCC_TIM6_CLK_ENABLE).Times(1);
	}
	coredactimer._mspInitCallback();
}

TEST_F(CoreDACTimerTest, _mspDeInitCallback)
{
	{
		EXPECT_CALL(halmock,HAL_RCC_TIM6_CLK_DISABLE).Times(1);
	}
	coredactimer._mspDeInitCallback();
}
