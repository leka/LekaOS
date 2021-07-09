// // Leka - LekaOS
// // Copyright 2021 APF France handicap
// // SPDX-License-Identifier: Apache-2.0

#include "CoreDAC.h"

#include "gtest/gtest.h"
#include "mocks/leka/LKCoreSTM32Hal.h"

using namespace leka;
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::InSequence;
using ::testing::Return;

class CoreDACTest : public ::testing::Test
{
  protected:
	CoreDACTest() : coredac(halmock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	LKCoreSTM32HalMock halmock;
	CoreDACTimer coredac;


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

TEST_F(CoreDACTest, instantiation)
{
	ASSERT_NE(&coredac, nullptr);
}

TEST_F(CoreDACTest, handleConfigurationInstance)
{
	auto hdac = coredac.getHandle();
    auto hdma = coredac.getDMAHandle();

	ASSERT_EQ(hdac.Instance, DAC);
    ASSERT_EQ(hdma.Instance, DMA1_Stream5);
}

TEST_F(CoreDACTest, initialize)
{
    {
        InSequence seq;

        EXPECT_CALL(halmock,HAL_RCC_GPIOA_CLK_ENABLE ).Times(1);
        EXPECT_CALL(halmock, HAL_RCC_DMA1_CLK_ENABLE).Times(1);
        EXPECT_CALL(halmock, HAL_NVIC_SetPriority).Times(1);
        EXPECT_CALL(halmock,HAL_NVIC_EnableIRQ ).Times(1);
        EXPECT_CALL(coredac, _registerMspCallbacks).Times(1);
        EXPECT_CALL(halmock,HAL_DAC_Init ).Times(1);
        EXPECT_CALL(coredac, _registerCallbacks).Times(1);
        EXPECT_CALL(halmock,HAL_DAC_ConfigChannel ).Times(1);
    }
    coredac.initialize();
}

TEST_F(CoreDACTest, deInitialize)
{
    {
        EXPECT_CALL(halmock,HAL_DAC_DeInit ).Times(1);
    }
    coredac.deInitialize();
}

TEST_F(CoreDACTest, start)
{
    {
        InSequence seq;

        EXPECT_CALL(halmock,HAL_DAC_Start ).Times(1);
        EXPECT_CALL(halmock,HAL_DAC_Start_DMA ).Times(1);
    }
    coredac.start();
}


TEST_F(CoreDACTest, stop)
{
    {
        EXPECT_CALL(halmock,HAL_DAC_Stop ).Times(1);
    }
    coredac.stop();
}

TEST_F(CoreDACTest, _registerCallbacks)
{
    {
        EXPECT_CALL(halmock,HAL_DAC_RegisterCallback ).Times(2);
    }
    coredac._registerCallbacks();
}

TEST_F(CoreDACTest, _halfCptCallback)
{

    coredac._halfCptCallback();
    ASSERT_EQ(coredac.dmaFlag(),interface::Dac::DMA_Flag::Half_cpt);
}

TEST_F(CoreDACTest, _cptCallback)
{
    coredac._cptCallback();

    ASSERT_EQ(coredac.dmaFlag(),interface::Dac::DMA_Flag::Cpt);
}

TEST_F(CoreDACTest, _registerMSPCallbacks)
{
    {
        EXPECT_CALL(halmock,HAL_DAC_RegisterCallback ).Times(2);
    }
    coredac._registerCallbacks();
}


TEST_F(CoreDACTest, _mspInitCallback)
{
    DMA_HandleTypeDef _hdma = coredac.getDMAHandle();

    {
        InSequence seq;

        EXPECT_CALL(halmock,HAL_RCC_DAC_CLK_ENABLE ).Times(1);
        EXPECT_CALL(halmock,HAL_RCC_GPIOA_CLK_ENABLE ).Times(1);
        EXPECT_CALL(halmock,HAL_GPIO_Init ).Times(1);
        EXPECT_CALL(halmock,HAL_DMA_Init).Times(1);
    }
    coredac._mspInitCallback();

    ASSERT_EQ(coredac.dmaFlag(),interface::Dac::DMA_Flag::Cpt);


    ASSERT_EQ(_hdma.Init.Channel, DMA_CHANNEL_7);
    ASSERT_EQ(_hdma.Init.Direction, DMA_MEMORY_TO_PERIPH);
    ASSERT_EQ(_hdma.Init.PeriphInc, DMA_PINC_DISABLE);
    ASSERT_EQ(_hdma.Init.MemInc, DMA_MINC_ENABLE);
    ASSERT_EQ(_hdma.Init.PeriphDataAlignment, DMA_PDATAALIGN_HALFWORD);
    ASSERT_EQ(_hdma.Init.MemDataAlignment, DMA_MDATAALIGN_HALFWORD);
    ASSERT_EQ(_hdma.Init.Mode, DMA_CIRCULAR);
    ASSERT_EQ(_hdma.Init.Priority, DMA_PRIORITY_LOW);		// changed from low to high
    ASSERT_EQ(_hdma.Init.FIFOMode, DMA_FIFOMODE_ENABLE);	// enable
    ASSERT_EQ(_hdma.Init.FIFOThreshold, DMA_FIFO_THRESHOLD_HALFFULL);
    ASSERT_EQ(_hdma.Init.MemBurst, DMA_MBURST_INC4);	// inc4
    ASSERT_EQ(_hdma.Init.PeriphBurst, DMA_PBURST_SINGLE);

    ASSERT_EQ(coredac.DMA_Handle1, &_hdma);
}

TEST_F(CoreDACTest, _mspDeInitCallback)
{
    {
        InSequence seq;

        EXPECT_CALL(halmock,HAL_RCC_DAC_CLK_DISABLE ).Times(1);
        EXPECT_CALL(halmock,HAL_GPIO_DeInit ).Times(1);
        EXPECT_CALL(halmock,HAL_DMA_DeInit ).Times(1);
    }
    coredac._mspDeInitCallback();
}



