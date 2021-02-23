// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreDSI.h"

#include "corevideo_config.h"
#include "gtest/gtest.h"
#include "mock_LKCoreSTM32Hal.h"

using namespace leka;
using ::testing::_;
using ::testing::InSequence;

class LKCoreDSITest : public ::testing::Test
{
  protected:
	LKCoreDSITest() : coredsi(halmock) {}

	void SetUp() override {}
	void TearDown() override {}

	LKCoreSTM32HalMock halmock;
	LKCoreDSI coredsi;
};

TEST_F(LKCoreDSITest, instantiation)
{
	ASSERT_NE(&coredsi, nullptr);
}

TEST_F(LKCoreDSITest, handleConfigurationInstance)
{
	auto handle = coredsi.getHandle();

	ASSERT_EQ(handle.Instance, DSI);
}

TEST_F(LKCoreDSITest, handleConfigurationInit)
{
	auto handle = coredsi.getHandle();

	ASSERT_EQ(handle.Init.NumberOfLanes, DSI_TWO_DATA_LANES);
	ASSERT_EQ(handle.Init.TXEscapeCkdiv, dsi::txEscapeClockDiv);
}

TEST_F(LKCoreDSITest, handleConfigurationVideoConfigGeneral)
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
	ASSERT_EQ(config.PacketSize, lcd::property.HACT);
}

TEST_F(LKCoreDSITest, handleConfigurationVideoConfigHorizontal)
{
	auto config = coredsi.getConfig();

	auto horizontalSyncActive = (lcd::property.HSA * dsi::laneByteClock_kHz) / dsi::lcdClock;
	auto horizontalBackPorch  = (lcd::property.HBP * dsi::laneByteClock_kHz) / dsi::lcdClock;
	auto horizontalLine =
		((lcd::property.HACT + lcd::property.HSA + lcd::property.HBP + lcd::property.HFP) * dsi::laneByteClock_kHz) /
		dsi::lcdClock;

	ASSERT_EQ(config.HorizontalSyncActive, horizontalSyncActive);
	ASSERT_EQ(config.HorizontalBackPorch, horizontalBackPorch);
	ASSERT_EQ(config.HorizontalLine, horizontalLine);
}

TEST_F(LKCoreDSITest, handleConfigurationVideoConfigVertical)
{
	auto config = coredsi.getConfig();

	ASSERT_EQ(config.VerticalSyncActive, lcd::property.VSA);
	ASSERT_EQ(config.VerticalBackPorch, lcd::property.VBP);
	ASSERT_EQ(config.VerticalFrontPorch, lcd::property.VFP);
	ASSERT_EQ(config.VerticalActive, lcd::property.VACT);
}

TEST_F(LKCoreDSITest, handleConfigurationVideoConfigLowPower)
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

TEST_F(LKCoreDSITest, initializationSequence)
{
	{
		InSequence seq;
		EXPECT_CALL(halmock, HAL_DSI_DeInit).Times(1);
		EXPECT_CALL(halmock, HAL_DSI_Init).Times(1);
		EXPECT_CALL(halmock, HAL_DSI_ConfigVideoMode).Times(1);
	}

	coredsi.initialize();
}

TEST_F(LKCoreDSITest, start)
{
	EXPECT_CALL(halmock, HAL_DSI_Start).Times(1);

	coredsi.start();
}

TEST_F(LKCoreDSITest, resetSequence)
{
	{
		InSequence seq;
		EXPECT_CALL(halmock, HAL_GPIO_Init).Times(1);
		EXPECT_CALL(halmock, HAL_GPIO_WritePin).Times(2);
	}

	coredsi.reset();
}

TEST_F(LKCoreDSITest, ioWriteCmdShortCommand)
{
	uint8_t command[2] = {0x2A, 0x2B};
	auto config		   = coredsi.getConfig();

	EXPECT_CALL(halmock, HAL_DSI_ShortWrite(_, config.VirtualChannelID, _, command[0], command[1])).Times(2);
	EXPECT_CALL(halmock, HAL_DSI_LongWrite).Times(0);

	coredsi.writeCommand((uint8_t *)command, 0);
	coredsi.writeCommand((uint8_t *)command, 1);
}

TEST_F(LKCoreDSITest, ioWriteCmdLongCommand)
{
	const uint8_t instruction = 0x42;
	auto config				  = coredsi.getConfig();

	EXPECT_CALL(halmock, HAL_DSI_ShortWrite).Times(0);

	for (int n_params = 2; n_params < 16; n_params++) {
		uint8_t command[n_params + 1];
		for (uint8_t i = 0; i < n_params; i++) {
			command[i] = i;
		}
		command[n_params] = instruction;

		EXPECT_CALL(halmock, HAL_DSI_LongWrite(_, config.VirtualChannelID, DSI_DCS_LONG_PKT_WRITE, n_params,
											   instruction, command))
			.Times(1);

		coredsi.writeCommand((uint8_t *)command, n_params);
	}
}
