// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLTDC.hpp"

#include "gtest/gtest.h"
#include "internal/corevideo_config.h"
#include "mocks/leka/CoreDSI.h"
#include "mocks/leka/CoreSTM32Hal.h"

using namespace leka;
using ::testing::_;
using ::testing::Field;
using ::testing::InSequence;
using ::testing::Pointee;
using ::testing::Property;

class CoreLTDCTest : public ::testing::Test
{
  protected:
	CoreLTDCTest() : coreltdc(halmock, dsimock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	CoreSTM32HalMock halmock;
	mock::CoreDSI dsimock;
	CoreLTDC coreltdc;
};

TEST_F(CoreLTDCTest, instantiation)
{
	ASSERT_NE(&coreltdc, nullptr);
}

TEST_F(CoreLTDCTest, handleConfigurationInstance)
{
	auto handle = coreltdc.getHandle();

	ASSERT_EQ(handle.Instance, LTDC);
}

TEST_F(CoreLTDCTest, handleConfigurationLayerCfg)
{
	auto handle = coreltdc.getHandle();

	ASSERT_EQ(handle.LayerCfg->ImageWidth, lcd::dimension::width);
	ASSERT_EQ(handle.LayerCfg->ImageHeight, lcd::dimension::height);
}

TEST_F(CoreLTDCTest, handleConfigurationSetupTimingConfig)
{
	auto handle = coreltdc.getHandle();

	auto horizontal_sync	 = (lcd::property::HSA - 1);
	auto accumulated_HBP	 = (lcd::property::HSA + lcd::property::HBP - 1);
	auto accumulated_activeW = (lcd::dimension::width + lcd::property::HSA + lcd::property::HBP - 1);
	auto total_width = (lcd::dimension::width + lcd::property::HSA + lcd::property::HBP + lcd::property::HFP - 1);

	ASSERT_EQ(handle.Init.HorizontalSync, horizontal_sync);
	ASSERT_EQ(handle.Init.AccumulatedHBP, accumulated_HBP);
	ASSERT_EQ(handle.Init.AccumulatedActiveW, accumulated_activeW);
	ASSERT_EQ(handle.Init.TotalWidth, total_width);
}

TEST_F(CoreLTDCTest, handleConfigurationSetupBackgroundConfig)
{
	auto handle = coreltdc.getHandle();

	ASSERT_EQ(handle.Init.Backcolor.Blue, 0);
	ASSERT_EQ(handle.Init.Backcolor.Green, 0);
	ASSERT_EQ(handle.Init.Backcolor.Red, 0);
	ASSERT_EQ(handle.Init.PCPolarity, LTDC_PCPOLARITY_IPC);
}

TEST_F(CoreLTDCTest, setupLayerConfig)
{
	auto layer_config = coreltdc.getLayerConfig();

	ASSERT_EQ(layer_config.WindowX0, 0);
	ASSERT_EQ(layer_config.WindowX1, lcd::dimension::width);
	ASSERT_EQ(layer_config.WindowY0, 0);
	ASSERT_EQ(layer_config.WindowY1, lcd::dimension::height);

	ASSERT_EQ(layer_config.PixelFormat, LTDC_PIXEL_FORMAT_ARGB8888);

	ASSERT_EQ(layer_config.FBStartAdress, lcd::frame_buffer_address);

	ASSERT_EQ(layer_config.Alpha, 255);
	ASSERT_EQ(layer_config.Alpha0, 0);

	ASSERT_EQ(layer_config.Backcolor.Blue, 0);
	ASSERT_EQ(layer_config.Backcolor.Green, 0);
	ASSERT_EQ(layer_config.Backcolor.Red, 0);

	ASSERT_EQ(layer_config.BlendingFactor1, LTDC_BLENDING_FACTOR1_PAxCA);
	ASSERT_EQ(layer_config.BlendingFactor2, LTDC_BLENDING_FACTOR2_PAxCA);

	ASSERT_EQ(layer_config.ImageWidth, lcd::dimension::width);
	ASSERT_EQ(layer_config.ImageHeight, lcd::dimension::height);
}

MATCHER_P(WithStructEqualTo, expected, "Compare RCC_PeriphCLKInitTypeDef")
{
	if (arg->PeriphClockSelection != expected.PeriphClockSelection) {
		return false;
	}
	if (arg->PLLSAI.PLLSAIN != expected.PLLSAI.PLLSAIN) {
		return false;
	}
	if (arg->PLLSAI.PLLSAIR != expected.PLLSAI.PLLSAIR) {
		return false;
	}
	if (arg->PLLSAIDivR != expected.PLLSAIDivR) {
		return false;
	}
	return true;
}

TEST_F(CoreLTDCTest, initializationSequence)
{
	uint8_t default_layer_id = 1;	// Foreground

	RCC_PeriphCLKInitTypeDef expected;

	expected.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	expected.PLLSAI.PLLSAIN		  = 384;
	expected.PLLSAI.PLLSAIR		  = 7;
	expected.PLLSAIDivR			  = RCC_PLLSAIDIVR_2;

	{
		InSequence seq;
		EXPECT_CALL(halmock, HAL_RCCEx_PeriphCLKConfig(WithStructEqualTo(expected))).Times(1);
		EXPECT_CALL(dsimock, getConfig).Times(1);
		EXPECT_CALL(halmock, HAL_LTDC_StructInitFromVideoConfig).Times(1);
		EXPECT_CALL(halmock, HAL_LTDC_Init).Times(1);
		EXPECT_CALL(halmock, HAL_LTDC_ConfigLayer(_, _, default_layer_id)).Times(1);
	}

	coreltdc.initialize();
}
