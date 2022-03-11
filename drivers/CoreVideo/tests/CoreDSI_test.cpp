// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDSI.hpp"

#include "gtest/gtest.h"
#include "internal/corevideo_config.h"
#include "mocks/leka/CoreSTM32Hal.h"

using namespace leka;
using ::testing::_;
using ::testing::InSequence;

class CoreDSITest : public ::testing::Test
{
  protected:
	CoreDSITest() : coredsi(halmock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	mock::CoreSTM32Hal halmock;
	CoreDSI coredsi;
};

TEST_F(CoreDSITest, instantiation)
{
	ASSERT_NE(&coredsi, nullptr);
}

TEST_F(CoreDSITest, handleConfigurationInstance)
{
	auto handle = coredsi.getHandle();

	ASSERT_EQ(handle.Instance, DSI);
}

TEST_F(CoreDSITest, handleConfigurationInit)
{
	auto handle = coredsi.getHandle();

	ASSERT_EQ(handle.Init.NumberOfLanes, DSI_TWO_DATA_LANES);
	ASSERT_EQ(handle.Init.TXEscapeCkdiv, dsi::txEscapeClockDiv);
}

TEST_F(CoreDSITest, handleConfigurationVideoConfigGeneral)
{
	auto config = coredsi.getConfig();

	ASSERT_EQ(config.VirtualChannelID, 0);
	ASSERT_EQ(config.ColorCoding, DSI_RGB888);
	ASSERT_EQ(config.VSPolarity, DSI_VSYNC_ACTIVE_HIGH);
	ASSERT_EQ(config.HSPolarity, DSI_HSYNC_ACTIVE_HIGH);
	ASSERT_EQ(config.DEPolarity, DSI_DATA_ENABLE_ACTIVE_HIGH);
	ASSERT_EQ(config.Mode, DSI_VID_MODE_BURST);
	ASSERT_EQ(config.NullPacketSize, 0xFFF);
	ASSERT_EQ(config.NumberOfChunks, 0);
	ASSERT_EQ(config.PacketSize, lcd::property::HACT);
}

TEST_F(CoreDSITest, handleConfigurationVideoConfigHorizontal)
{
	auto config = coredsi.getConfig();

	auto horizontalSyncActive = (lcd::property::HSA * dsi::laneByteClock_kHz) / dsi::lcdClock;
	auto horizontalBackPorch  = (lcd::property::HBP * dsi::laneByteClock_kHz) / dsi::lcdClock;
	auto horizontalLine		  = ((lcd::property::HACT + lcd::property::HSA + lcd::property::HBP + lcd::property::HFP) *
							 dsi::laneByteClock_kHz) /
						  dsi::lcdClock;

	ASSERT_EQ(config.HorizontalSyncActive, horizontalSyncActive);
	ASSERT_EQ(config.HorizontalBackPorch, horizontalBackPorch);
	ASSERT_EQ(config.HorizontalLine, horizontalLine);
}

TEST_F(CoreDSITest, handleConfigurationVideoConfigVertical)
{
	auto config = coredsi.getConfig();

	ASSERT_EQ(config.VerticalSyncActive, lcd::property::VSA);
	ASSERT_EQ(config.VerticalBackPorch, lcd::property::VBP);
	ASSERT_EQ(config.VerticalFrontPorch, lcd::property::VFP);
	ASSERT_EQ(config.VerticalActive, lcd::property::VACT);
}

TEST_F(CoreDSITest, handleConfigurationVideoConfigLowPower)
{
	auto config = coredsi.getConfig();

	ASSERT_EQ(config.LPCommandEnable, DSI_LP_COMMAND_ENABLE);
	ASSERT_EQ(config.LPLargestPacketSize, 16);
	ASSERT_EQ(config.LPVACTLargestPacketSize, 0);
	ASSERT_EQ(config.LPHorizontalFrontPorchEnable, DSI_LP_HFP_ENABLE);
	ASSERT_EQ(config.LPHorizontalBackPorchEnable, DSI_LP_HBP_ENABLE);
	ASSERT_EQ(config.LPVerticalActiveEnable, DSI_LP_VACT_ENABLE);
	ASSERT_EQ(config.LPVerticalFrontPorchEnable, DSI_LP_VFP_ENABLE);
	ASSERT_EQ(config.LPVerticalBackPorchEnable, DSI_LP_VBP_ENABLE);
	ASSERT_EQ(config.LPVerticalSyncActiveEnable, DSI_LP_VSYNC_ENABLE);
}

TEST_F(CoreDSITest, initializationSequence)
{
	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_RCC_DSI_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_DSI_FORCE_RESET).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_DSI_RELEASE_RESET).Times(1);

		EXPECT_CALL(halmock, HAL_NVIC_SetPriority(DSI_IRQn, _, _)).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ(DSI_IRQn)).Times(1);

		EXPECT_CALL(halmock, HAL_RCC_GPIOJ_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_GPIO_Init).Times(1);
		EXPECT_CALL(halmock, HAL_GPIO_WritePin).Times(2);

		EXPECT_CALL(halmock, HAL_DSI_DeInit).Times(1);
		EXPECT_CALL(halmock, HAL_DSI_Init).Times(1);
		EXPECT_CALL(halmock, HAL_DSI_ConfigVideoMode).Times(1);
	}

	coredsi.initialize();
}

TEST_F(CoreDSITest, start)
{
	EXPECT_CALL(halmock, HAL_DSI_Start).Times(1);

	coredsi.start();
}

TEST_F(CoreDSITest, resetSequence)
{
	{
		InSequence seq;
		EXPECT_CALL(halmock, HAL_RCC_GPIOJ_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_GPIO_Init).Times(1);
		EXPECT_CALL(halmock, HAL_GPIO_WritePin).Times(2);
	}

	coredsi.reset();
}

TEST_F(CoreDSITest, ioWriteShortCommand)
{
	uint8_t command[] = {0x2A, 0x2B};
	auto config		  = coredsi.getConfig();

	EXPECT_CALL(halmock, HAL_DSI_ShortWrite(_, config.VirtualChannelID, _, command[0], command[1])).Times(1);
	EXPECT_CALL(halmock, HAL_DSI_LongWrite).Times(0);

	coredsi.write(command, std::size(command));
}

TEST_F(CoreDSITest, ioWriteLongCommand)
{
	auto config = coredsi.getConfig();

	uint8_t command1[] = {0x2A, 0x2B, 0x2C};
	uint8_t command2[] = {0x2A, 0x2B, 0x2C, 0x2D, 0x2E};
	uint8_t command3[] = {0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30};
	uint8_t command4[] = {0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x31};

	EXPECT_CALL(halmock, HAL_DSI_ShortWrite).Times(0);

	EXPECT_CALL(halmock, HAL_DSI_LongWrite(_, config.VirtualChannelID, DSI_DCS_LONG_PKT_WRITE, std::size(command1),
										   command1[std::size(command1) - 1], command1))
		.Times(1);

	coredsi.write(command1, std::size(command1));

	EXPECT_CALL(halmock, HAL_DSI_LongWrite(_, config.VirtualChannelID, DSI_DCS_LONG_PKT_WRITE, std::size(command2),
										   command2[std::size(command2) - 1], command2))
		.Times(1);

	coredsi.write(command2, std::size(command2));

	EXPECT_CALL(halmock, HAL_DSI_LongWrite(_, config.VirtualChannelID, DSI_DCS_LONG_PKT_WRITE, std::size(command3),
										   command3[std::size(command3) - 1], command3))
		.Times(1);

	coredsi.write(command3, std::size(command3));

	EXPECT_CALL(halmock, HAL_DSI_LongWrite(_, config.VirtualChannelID, DSI_DCS_LONG_PKT_WRITE, std::size(command4),
										   command4[std::size(command4) - 1], command4))
		.Times(1);

	coredsi.write(command4, std::size(command4));
}
