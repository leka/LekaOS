// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreSTM32HalBasicTimer.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreSTM32Hal.h"
#include "mocks/leka/STM32HalBasicTimer.h"

using namespace leka;

using testing::_;
using ::testing::DoAll;
using ::testing::InSequence;
using ::testing::MockFunction;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::SaveArgPointee;

class CoreSTM32HalBasicTimerTest : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		basic_timer.registerCallback(callback.AsStdFunction());

		EXPECT_CALL(halmock, HAL_TIM_RegisterCallback(_, HAL_TIM_BASE_MSPINIT_CB_ID, _))
			.WillOnce(DoAll(SaveArg<2>(&mspinit_callback), Return(HAL_StatusTypeDef::HAL_OK)));
		EXPECT_CALL(halmock, HAL_TIM_RegisterCallback(_, HAL_TIM_BASE_MSPDEINIT_CB_ID, _))
			.WillOnce(DoAll(SaveArg<2>(&mspdeinit_callback), Return(HAL_StatusTypeDef::HAL_OK)));
		EXPECT_CALL(halmock, HAL_TIM_RegisterCallback(_, HAL_TIM_PERIOD_ELAPSED_CB_ID, _))
			.WillOnce(DoAll(SaveArg<2>(&period_elapsed_callback), Return(HAL_StatusTypeDef::HAL_OK)));

		EXPECT_CALL(halmock, HAL_TIM_Base_Init);
		EXPECT_CALL(halmock, HAL_TIMEx_MasterConfigSynchronization)
			.WillOnce(DoAll(SaveArgPointee<1>(&basic_timer_master_config), Return(HAL_StatusTypeDef::HAL_OK)));

		basic_timer.initialize();
	}
	// void TearDown() override {}

	mock::CoreSTM32Hal halmock;

	CoreSTM32HalBasicTimer basic_timer {halmock};
	TIM_MasterConfigTypeDef basic_timer_master_config {};

	MockFunction<void()> callback;

	std::function<void(TIM_HandleTypeDef *)> mspinit_callback		 = [](TIM_HandleTypeDef *) {};
	std::function<void(TIM_HandleTypeDef *)> mspdeinit_callback		 = [](TIM_HandleTypeDef *) {};
	std::function<void(TIM_HandleTypeDef *)> period_elapsed_callback = [](TIM_HandleTypeDef *) {};
};

TEST_F(CoreSTM32HalBasicTimerTest, initializationDefault)
{
	EXPECT_NE(&basic_timer, nullptr);

	auto handle = basic_timer.getHandle();
	EXPECT_NE(&handle, nullptr);
	auto allowed_instance = handle.Instance == TIM6 || handle.Instance == TIM7;
	EXPECT_TRUE(allowed_instance);
}

TEST_F(CoreSTM32HalBasicTimerTest, initialize)
{
	{
		InSequence seq;

		{
			EXPECT_CALL(halmock, HAL_TIM_RegisterCallback(_, HAL_TIM_BASE_MSPINIT_CB_ID, _));
			EXPECT_CALL(halmock, HAL_TIM_RegisterCallback(_, HAL_TIM_BASE_MSPDEINIT_CB_ID, _));
		}	// MSP callbacks

		{
			EXPECT_CALL(halmock, HAL_TIM_Base_Init);
			EXPECT_CALL(halmock, HAL_TIMEx_MasterConfigSynchronization);
		}	// BasicTimer config

		{
			EXPECT_CALL(halmock, HAL_TIM_RegisterCallback(_, HAL_TIM_PERIOD_ELAPSED_CB_ID, _));
		}	// Callback on event
	}

	basic_timer.initialize();
}

TEST_F(CoreSTM32HalBasicTimerTest, initializeMspInit)
{
	auto *basic_timer_instance = basic_timer.getHandle().Instance;

	if (basic_timer_instance == TIM6) {
		EXPECT_CALL(halmock, HAL_RCC_TIM6_CLK_ENABLE);
		EXPECT_CALL(halmock, HAL_NVIC_SetPriority(TIM6_DAC_IRQn, _, _));
		EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn));
	} else if (basic_timer_instance == TIM7) {
		EXPECT_CALL(halmock, HAL_RCC_TIM7_CLK_ENABLE);
		EXPECT_CALL(halmock, HAL_NVIC_SetPriority(TIM7_IRQn, _, _));
		EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ(TIM7_IRQn));
	} else {
		FAIL();	  // Only timers 6 and 7 can be used as Basic Timer
	}

	mspinit_callback(&basic_timer.getHandle());
}

TEST_F(CoreSTM32HalBasicTimerTest, initializeMspDeinit)
{
	auto *basic_timer_instance = basic_timer.getHandle().Instance;

	if (basic_timer_instance == TIM6) {
		EXPECT_CALL(halmock, HAL_RCC_TIM6_CLK_DISABLE);
	} else if (basic_timer_instance == TIM7) {
		EXPECT_CALL(halmock, HAL_RCC_TIM7_CLK_DISABLE);
	} else {
		FAIL();	  // Only timers 6 and 7 can be used as Basic Timer
	}

	mspdeinit_callback(&basic_timer.getHandle());
}

TEST_F(CoreSTM32HalBasicTimerTest, initializeConfig)
{
	auto timer_handle = basic_timer.getHandle();

	auto CK_INT				 = float(108'000'000.0);
	auto default_sample_rate = float(44'100.0);
	auto divider			 = std::round(CK_INT / default_sample_rate);

	// The prescaler can divide the counter clock frequency by any factor between 1 and 65536, see 28.3.1 of RM
	auto prescaler = timer_handle.Init.Prescaler;
	EXPECT_LE(prescaler, 0xFFF);

	// The counter counts from 0 to the auto-reload value, see 28.3.2 of RM
	auto auto_reload_value = timer_handle.Init.Period;
	EXPECT_GE(auto_reload_value, 1);
	EXPECT_LE(auto_reload_value, 0xFFF);

	// To fit required frequency (default sample rate), multiplying prescaler and auto-reload value must give
	// approximately the divider
	EXPECT_NEAR((prescaler + 1) * (auto_reload_value + 1), divider, 1);

	// Disable shadow write
	EXPECT_EQ(timer_handle.Init.AutoReloadPreload, TIM_AUTORELOAD_PRELOAD_DISABLE);

	// The update event is selected as a trigger output, see 28.4.2 of RM
	EXPECT_EQ(basic_timer_master_config.MasterOutputTrigger, TIM_TRGO_UPDATE);

	// ? Config not available for BasicTimer (TIM6 and TIM7)
	// _htim.Init.CounterMode;
	// _htim.Init.ClockDivision;
	// _htim.Init.RepetitionCounter;
	// timerMasterConfig.MasterSlaveMode;
}

TEST_F(CoreSTM32HalBasicTimerTest, initializePeriodElapsedCallback)
{
	EXPECT_CALL(callback, Call);
	period_elapsed_callback(&basic_timer.getHandle());
}

TEST_F(CoreSTM32HalBasicTimerTest, linkDACTimer)
{
	basic_timer.linkDACTimer(nullptr);

	DAC_ChannelConfTypeDef dac_config {};

	basic_timer.linkDACTimer(&dac_config);

	auto *basic_timer_instance = basic_timer.getHandle().Instance;
	if (basic_timer_instance == TIM6) {
		EXPECT_EQ(dac_config.DAC_Trigger, DAC_TRIGGER_T6_TRGO);
	} else if (basic_timer_instance == TIM7) {
		EXPECT_EQ(dac_config.DAC_Trigger, DAC_TRIGGER_T7_TRGO);
	} else {
		FAIL();	  // Only timers 6 and 7 can be used as DAC Timer
	}
}

TEST_F(CoreSTM32HalBasicTimerTest, terminate)
{
	EXPECT_CALL(halmock, HAL_TIM_Base_DeInit);

	basic_timer.terminate();
}

TEST_F(CoreSTM32HalBasicTimerTest, start)
{
	EXPECT_CALL(halmock, HAL_TIM_Base_Start_IT);

	basic_timer.start();
}

TEST_F(CoreSTM32HalBasicTimerTest, stop)
{
	EXPECT_CALL(halmock, HAL_TIM_Base_Stop_IT);

	basic_timer.stop();
}
