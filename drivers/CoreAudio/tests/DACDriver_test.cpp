// // Leka - LekaOS
// // Copyright 2021 APF France handicap
// // SPDX-License-Identifier: Apache-2.0

#include "DACDriver.h"

#include "gtest/gtest.h"
#include "mocks/leka/LKCoreSTM32Hal.h"

using namespace leka;
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::InSequence;
using ::testing::Return;

class DACDriver : public ::testing::Test
{
  protected:
	DACDriver() : dacDriver(halmock) {}

	LKCoreSTM32HalMock halmock;
	DACDriver dacDriver;
};

TEST_F(DACDriver, instantiation)
{
	ASSERT_NE(&dacDriver, nullptr);
}

TEST_F(DACDriver, handleConfigurationInstance)
{
	auto hdac = dacDriver.getHandle();
	auto hdma = dacDriver.getDMAHandle();

	ASSERT_EQ(hdac.Instance, DAC);
	ASSERT_EQ(hdma.Instance, DMA1_Stream5);
}

TEST_F(DACDriver, initialize)
{
	auto handle = dacDriver.getHandle();
	auto timer	= DACTimer(halmock, DACTimer::HardwareBasicTimer::BasicTimer6);
	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_RCC_GPIOA_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_DMA1_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_SetPriority).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ).Times(1);
		EXPECT_CALL(halmock, HAL_DAC_RegisterCallback).Times(2);
		EXPECT_CALL(halmock, HAL_DAC_Init).Times(1);
		EXPECT_CALL(halmock, HAL_DAC_RegisterCallback).Times(2);
		EXPECT_CALL(halmock, HAL_DAC_ConfigChannel).Times(1);
	}
	dacDriver.initialize(timer, nullptr, nullptr);
}

TEST_F(DACDriver, terminate)
{
	{
		EXPECT_CALL(halmock, HAL_DAC_DeInit).Times(1);
	}
	dacDriver.terminate();
}

// TEST_F(DACDriver, mspInitCallback)
// {
// 	{
// 		InSequence seq;

// 		EXPECT_CALL(halmock, HAL_RCC_DAC_CLK_ENABLE).Times(1);
// 		EXPECT_CALL(halmock, HAL_RCC_GPIOA_CLK_ENABLE).Times(1);
// 		EXPECT_CALL(halmock, HAL_GPIO_Init).Times(1);
// 		EXPECT_CALL(halmock, HAL_DMA_Init).Times(1);
// 	}
// 	dacDriver._mspInitCallback();

// 	auto hdma = dacDriver.getDMAHandle();

// 	ASSERT_EQ(hdma.Init.Channel, DMA_CHANNEL_7);
// 	ASSERT_EQ(hdma.Init.Direction, DMA_MEMORY_TO_PERIPH);
// 	ASSERT_EQ(hdma.Init.PeriphInc, DMA_PINC_DISABLE);
// 	ASSERT_EQ(hdma.Init.MemInc, DMA_MINC_ENABLE);
// 	ASSERT_EQ(hdma.Init.PeriphDataAlignment, DMA_PDATAALIGN_HALFWORD);
// 	ASSERT_EQ(hdma.Init.MemDataAlignment, DMA_MDATAALIGN_HALFWORD);
// 	ASSERT_EQ(hdma.Init.Mode, DMA_CIRCULAR);
// 	ASSERT_EQ(hdma.Init.Priority, DMA_PRIORITY_LOW);	  // changed from low to high
// 	ASSERT_EQ(hdma.Init.FIFOMode, DMA_FIFOMODE_ENABLE);	  // enable
// 	ASSERT_EQ(hdma.Init.FIFOThreshold, DMA_FIFO_THRESHOLD_HALFFULL);
// 	ASSERT_EQ(hdma.Init.MemBurst, DMA_MBURST_INC4);	  // inc4
// 	ASSERT_EQ(hdma.Init.PeriphBurst, DMA_PBURST_SINGLE);

// 	// NOT EASILY TESTABLE
// 	// auto hdac = dacDriver.getHandle();
// 	// ASSERT_EQ(hdac.DMA_Handle1, &hdma);
// }

// TEST_F(DACDriver, start)
// {
// 	{
// 		InSequence seq;

// 		EXPECT_CALL(halmock, HAL_DAC_Start).Times(1);
// 		EXPECT_CALL(halmock, HAL_DAC_Start_DMA).Times(1);
// 	}
// 	dacDriver.start(nullptr, 0);
// }

// TEST_F(DACDriver, stop)
// {
// 	{
// 		EXPECT_CALL(halmock, HAL_DAC_Stop_DMA).Times(1);
// 	}
// 	dacDriver.stop();
// }

// TEST_F(DACDriver, halfCptCallback)
// {
// 	dacDriver._halfCptCallback();
// 	ASSERT_EQ(dacDriver.dmaFlag(), interface::Dac::DMA_Flag::Half_cpt);
// }

// TEST_F(DACDriver, _cptCallback)
// {
// 	dacDriver._cptCallback();

// 	ASSERT_EQ(dacDriver.dmaFlag(), interface::Dac::DMA_Flag::Cpt);
// }

// TEST_F(DACDriver, mspDeInitCallback)
// {
// 	{
// 		InSequence seq;

// 		EXPECT_CALL(halmock, HAL_RCC_DAC_CLK_DISABLE).Times(1);
// 		EXPECT_CALL(halmock, HAL_GPIO_DeInit).Times(1);
// 		EXPECT_CALL(halmock, HAL_DMA_DeInit).Times(1);
// 	}
// 	dacDriver._mspDeInitCallback();
// }