// // Leka - LekaOS
// // Copyright 2021 APF France handicap
// // SPDX-License-Identifier: Apache-2.0

#include "CoreSDRAM.hpp"

#include "gtest/gtest.h"
#include "mocks/leka/CoreSTM32Hal.h"

using namespace leka;
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::InSequence;
using ::testing::Return;

class CoreSDRAMTest : public ::testing::Test
{
  protected:
	CoreSDRAMTest() : coresdram(halmock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	mock::CoreSTM32Hal halmock;
	CoreSDRAM coresdram;

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

TEST_F(CoreSDRAMTest, instantiation)
{
	ASSERT_NE(&coresdram, nullptr);
}

TEST_F(CoreSDRAMTest, handleConfigurationInstance)
{
	auto handle = coresdram.getHandle();

	ASSERT_EQ(handle.Instance, FMC_SDRAM_DEVICE);
}

TEST_F(CoreSDRAMTest, setupSDRAMConfiguration)
{
	coresdram.setupSDRAMConfig();
	auto handle = coresdram.getHandle();

	ASSERT_EQ(handle.Init.SDBank, FMC_SDRAM_BANK1);
	ASSERT_EQ(handle.Init.ColumnBitsNumber, FMC_SDRAM_COLUMN_BITS_NUM_8);
	ASSERT_EQ(handle.Init.RowBitsNumber, FMC_SDRAM_ROW_BITS_NUM_12);
	ASSERT_EQ(handle.Init.MemoryDataWidth, sdram::memory_width);
	ASSERT_EQ(handle.Init.InternalBankNumber, FMC_SDRAM_INTERN_BANKS_NUM_4);
	ASSERT_EQ(handle.Init.CASLatency, FMC_SDRAM_CAS_LATENCY_3);
	ASSERT_EQ(handle.Init.WriteProtection, FMC_SDRAM_WRITE_PROTECTION_DISABLE);
	ASSERT_EQ(handle.Init.SDClockPeriod, sdram::sd_clock_period);
	ASSERT_EQ(handle.Init.ReadBurst, FMC_SDRAM_RBURST_ENABLE);
	ASSERT_EQ(handle.Init.ReadPipeDelay, FMC_SDRAM_RPIPE_DELAY_0);
}

TEST_F(CoreSDRAMTest, setupTimingConfiguration)
{
	FMC_SDRAM_TimingTypeDef actual_timing = coresdram.setupTimingConfig();

	ASSERT_EQ(actual_timing.LoadToActiveDelay, 2);
	ASSERT_EQ(actual_timing.ExitSelfRefreshDelay, 7);
	ASSERT_EQ(actual_timing.SelfRefreshTime, 4);
	ASSERT_EQ(actual_timing.RowCycleDelay, 7);
	ASSERT_EQ(actual_timing.WriteRecoveryTime, 2);
	ASSERT_EQ(actual_timing.RPDelay, 2);
	ASSERT_EQ(actual_timing.RCDDelay, 2);
}

TEST_F(CoreSDRAMTest, setupDMA)
{
	DMA_HandleTypeDef actual_dma = coresdram.setupDMA();

	ASSERT_EQ(actual_dma.Instance, DMA2_Stream0);
	ASSERT_EQ(actual_dma.Init.Channel, DMA_CHANNEL_0);
	ASSERT_EQ(actual_dma.Init.Direction, DMA_MEMORY_TO_MEMORY);
	ASSERT_EQ(actual_dma.Init.PeriphInc, DMA_PINC_ENABLE);
	ASSERT_EQ(actual_dma.Init.MemInc, DMA_MINC_ENABLE);
	ASSERT_EQ(actual_dma.Init.PeriphDataAlignment, DMA_PDATAALIGN_WORD);
	ASSERT_EQ(actual_dma.Init.MemDataAlignment, DMA_MDATAALIGN_WORD);
	ASSERT_EQ(actual_dma.Init.Mode, DMA_NORMAL);
	ASSERT_EQ(actual_dma.Init.Priority, DMA_PRIORITY_HIGH);
	ASSERT_EQ(actual_dma.Init.FIFOMode, DMA_FIFOMODE_DISABLE);
	ASSERT_EQ(actual_dma.Init.FIFOThreshold, DMA_FIFO_THRESHOLD_FULL);
	ASSERT_EQ(actual_dma.Init.MemBurst, DMA_MBURST_SINGLE);
	ASSERT_EQ(actual_dma.Init.PeriphBurst, DMA_PBURST_SINGLE);
}

TEST_F(CoreSDRAMTest, initializeController)
{
	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_RCC_FMC_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_DMA2_CLK_ENABLE).Times(1);

		EXPECT_CALL(halmock, HAL_RCC_GPIOD_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_GPIOE_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_GPIOF_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_GPIOG_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_GPIOH_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_GPIOI_CLK_ENABLE).Times(1);

		EXPECT_CALL(halmock, HAL_GPIO_Init(GPIOD, _)).Times(1);
		EXPECT_CALL(halmock, HAL_GPIO_Init(GPIOE, _)).Times(1);
		EXPECT_CALL(halmock, HAL_GPIO_Init(GPIOF, _)).Times(1);
		EXPECT_CALL(halmock, HAL_GPIO_Init(GPIOG, _)).Times(1);
		EXPECT_CALL(halmock, HAL_GPIO_Init(GPIOH, _)).Times(1);
		EXPECT_CALL(halmock, HAL_GPIO_Init(GPIOI, _)).Times(1);

		EXPECT_CALL(halmock, HAL_LINKDMA).Times(1);
		EXPECT_CALL(halmock, HAL_DMA_DeInit).Times(1);
		EXPECT_CALL(halmock, HAL_DMA_Init).Times(1);

		EXPECT_CALL(halmock, HAL_NVIC_SetPriority).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ).Times(1);
	}

	coresdram.initializeController();
}

TEST_F(CoreSDRAMTest, initializationSequence)
{
	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_SDRAM_SendCommand).Times(4);
		EXPECT_CALL(halmock, HAL_SDRAM_ProgramRefreshRate).Times(1);
	}

	coresdram.initializationSequence();
}

TEST_F(CoreSDRAMTest, initializeSDRAMInitSuccess)
{
	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_RCC_FMC_CLK_ENABLE).Times(1);	 // Check call of mspInit
		EXPECT_CALL(halmock, HAL_SDRAM_Init).WillOnce(Return(HAL_OK));
		EXPECT_CALL(halmock, HAL_SDRAM_ProgramRefreshRate).Times(1);   // Check call of initializationSequence
	}

	silenceUnexpectedCalls();

	auto actual_status = coresdram.initialize();

	ASSERT_EQ(actual_status, sdram::status::ok);
}

TEST_F(CoreSDRAMTest, initializeSDRAMInitFailed)
{
	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_RCC_FMC_CLK_ENABLE).Times(1);	 // Check call of mspInit
		EXPECT_CALL(halmock, HAL_SDRAM_Init).WillOnce(Return(HAL_ERROR));
		EXPECT_CALL(halmock, HAL_SDRAM_ProgramRefreshRate).Times(1);   // Check call of initializationSequence
	}

	silenceUnexpectedCalls();

	auto actual_status = coresdram.initialize();

	ASSERT_EQ(actual_status, sdram::status::error);
}
