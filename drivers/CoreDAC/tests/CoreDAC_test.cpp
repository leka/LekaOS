// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDAC.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/STM32HalTimer.h"
#include "mocks/leka/CoreSTM32Hal.h"

using namespace leka;

using testing::_;
using ::testing::DoAll;
using ::testing::InSequence;
using ::testing::MockFunction;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::SaveArgPointee;

class CoreDACTest : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		dac.registerDMACallbacks(on_half_transfer_callback.AsStdFunction(),
								 on_complete_transfer_callback.AsStdFunction());

		EXPECT_CALL(halmock, HAL_DAC_RegisterCallback(_, HAL_DAC_MSPINIT_CB_ID, _))
			.WillOnce(DoAll(SaveArg<2>(&mspinit_callback), Return(HAL_StatusTypeDef::HAL_OK)));
		EXPECT_CALL(halmock, HAL_DAC_RegisterCallback(_, HAL_DAC_MSPDEINIT_CB_ID, _))
			.WillOnce(DoAll(SaveArg<2>(&mspdeinit_callback), Return(HAL_StatusTypeDef::HAL_OK)));
		EXPECT_CALL(halmock, HAL_DAC_RegisterCallback(_, HAL_DAC_CH1_HALF_COMPLETE_CB_ID, _))
			.WillOnce(DoAll(SaveArg<2>(&dma_half_transfer_callback), Return(HAL_StatusTypeDef::HAL_OK)));
		EXPECT_CALL(halmock, HAL_DAC_RegisterCallback(_, HAL_DAC_CH1_COMPLETE_CB_ID, _))
			.WillOnce(DoAll(SaveArg<2>(&dma_complete_transfer_callback), Return(HAL_StatusTypeDef::HAL_OK)));

		EXPECT_CALL(halmock, HAL_DAC_Init);
		EXPECT_CALL(haltimermock, linkDACTimer);
		EXPECT_CALL(halmock, HAL_DAC_ConfigChannel)
			.WillOnce(DoAll(SaveArgPointee<1>(&dac_config), Return(HAL_StatusTypeDef::HAL_OK)));

		dac.initialize();
	}
	// void TearDown() override {}

	mock::CoreSTM32Hal halmock;
	mock::STM32HalTimer haltimermock;

	CoreDAC dac {halmock, haltimermock};

	MockFunction<void()> on_half_transfer_callback;
	MockFunction<void()> on_complete_transfer_callback;

	std::function<void(DAC_HandleTypeDef *)> mspinit_callback	= [](DAC_HandleTypeDef *) {};
	std::function<void(DAC_HandleTypeDef *)> mspdeinit_callback = [](DAC_HandleTypeDef *) {};

	DAC_ChannelConfTypeDef dac_config {};

	std::function<void(DAC_HandleTypeDef *)> dma_half_transfer_callback		= [](DAC_HandleTypeDef *) {};
	std::function<void(DAC_HandleTypeDef *)> dma_complete_transfer_callback = [](DAC_HandleTypeDef *) {};
};

TEST_F(CoreDACTest, initializationDefault)
{
	EXPECT_NE(&dac, nullptr);

	auto handle = dac.getHandle();
	EXPECT_NE(&handle, nullptr);
	EXPECT_EQ(handle.Instance, DAC);
}

TEST_F(CoreDACTest, initialize)
{
	{
		InSequence seq;

		{
			EXPECT_CALL(halmock, HAL_DAC_RegisterCallback(_, HAL_DAC_MSPINIT_CB_ID, _));
			EXPECT_CALL(halmock, HAL_DAC_RegisterCallback(_, HAL_DAC_MSPDEINIT_CB_ID, _));
		}	// MSP callbacks

		{
			EXPECT_CALL(halmock, HAL_DAC_Init);
			EXPECT_CALL(haltimermock, linkDACTimer);
			EXPECT_CALL(halmock, HAL_DAC_ConfigChannel);
		}	// DAC config

		{
			EXPECT_CALL(halmock, HAL_DAC_RegisterCallback(_, HAL_DAC_CH1_HALF_COMPLETE_CB_ID, _));
			EXPECT_CALL(halmock, HAL_DAC_RegisterCallback(_, HAL_DAC_CH1_COMPLETE_CB_ID, _));
		}	// DAC DMA config
	}

	dac.initialize();
}

TEST_F(CoreDACTest, initializeMspInit)
{
	EXPECT_CALL(halmock, HAL_RCC_DMA1_CLK_ENABLE);
	EXPECT_CALL(halmock, HAL_NVIC_SetPriority);
	EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ);
	EXPECT_CALL(halmock, HAL_DMA_Init);

	EXPECT_CALL(halmock, HAL_RCC_DAC_CLK_ENABLE);

	mspinit_callback(&dac.getHandle());
}

TEST_F(CoreDACTest, initializeMspDeinit)
{
	EXPECT_CALL(halmock, HAL_DMA_DeInit);
	EXPECT_CALL(halmock, HAL_RCC_DAC_CLK_DISABLE);
	mspdeinit_callback(&dac.getHandle());
}

TEST_F(CoreDACTest, initializeMspInitDMAConfig)
{
	auto dma_handle = DMA_HandleTypeDef {};

	EXPECT_CALL(halmock, HAL_RCC_DMA1_CLK_ENABLE);
	EXPECT_CALL(halmock, HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, _, _));
	EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn));
	EXPECT_CALL(halmock, HAL_DMA_Init)
		.WillOnce(DoAll(SaveArgPointee<0>(&dma_handle), Return(HAL_StatusTypeDef::HAL_OK)));

	EXPECT_CALL(halmock, HAL_RCC_DAC_CLK_ENABLE);

	mspinit_callback(&dac.getHandle());

	EXPECT_NE(&dma_handle, nullptr);
	EXPECT_EQ(dma_handle.Instance, DMA1_Stream5);

	EXPECT_EQ(dma_handle.Init.Channel, DMA_CHANNEL_7);
	EXPECT_EQ(dma_handle.Init.Direction, DMA_MEMORY_TO_PERIPH);
}

TEST_F(CoreDACTest, initializeConfig)
{
	// necessary to reach the full voltage range in DAC output
	EXPECT_EQ(dac_config.DAC_OutputBuffer, DAC_OUTPUTBUFFER_ENABLE);
}

TEST_F(CoreDACTest, initializeDmaHalfTransferCallback)
{
	EXPECT_CALL(on_half_transfer_callback, Call);
	dma_half_transfer_callback(&dac.getHandle());
}

TEST_F(CoreDACTest, initializeDmaCompleteTransferCallback)
{
	EXPECT_CALL(on_complete_transfer_callback, Call);
	dma_complete_transfer_callback(&dac.getHandle());
}

TEST_F(CoreDACTest, terminate)
{
	EXPECT_CALL(halmock, HAL_DAC_DeInit);

	dac.terminate();
}

TEST_F(CoreDACTest, start)
{
	auto data = std::array<uint16_t, 3> {0x01, 0x02, 0x03};

	dac.registerDataToPlay(data);

	EXPECT_CALL(haltimermock, start);
	EXPECT_CALL(halmock, HAL_DAC_Start_DMA(_, _, reinterpret_cast<uint32_t *>(data.data()), _, _));

	dac.start();
}

TEST_F(CoreDACTest, stop)
{
	EXPECT_CALL(halmock, HAL_DAC_Stop_DMA);

	dac.stop();
}
