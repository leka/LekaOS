// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreJPEG.hpp"

#include "external/st_jpeg_utils.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreJPEGMode.h"
#include "mocks/leka/CoreSTM32Hal.h"
#include "mocks/leka/File.h"

using namespace leka;

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::DoAll;
using ::testing::InSequence;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArrayArgument;

class CoreJPEGTest : public ::testing::Test
{
  protected:
	CoreJPEGTest() : corejpeg(halmock, modemock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	mock::CoreSTM32Hal halmock;
	mock::CoreJPEGMode modemock;
	mock::File filemock;
	CoreJPEG corejpeg;
};

TEST_F(CoreJPEGTest, instantiation)
{
	ASSERT_NE(&corejpeg, nullptr);
}

TEST_F(CoreJPEGTest, handleConfigurationInstance)
{
	auto handle = corejpeg.getHandle();

	ASSERT_EQ(handle.Instance, JPEG);
}

TEST_F(CoreJPEGTest, getImageProperties)
{
	auto expected_properties = JPEGImageProperties {};

	expected_properties.ImageWidth	= 800;
	expected_properties.ImageHeight = 480;

	EXPECT_CALL(halmock, HAL_JPEG_GetInfo(_, _)).WillOnce(DoAll(SetArgPointee<1>(expected_properties), Return(HAL_OK)));

	auto actual_properties = corejpeg.getImageProperties();

	EXPECT_EQ(actual_properties.ImageWidth, expected_properties.ImageWidth);
	EXPECT_EQ(actual_properties.ImageHeight, expected_properties.ImageHeight);
	EXPECT_EQ(actual_properties.getWidthOffset(), expected_properties.getWidthOffset());
}

TEST_F(CoreJPEGTest, initializationSequence)
{
	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_RCC_JPEG_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_JPEG_FORCE_RESET).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_JPEG_RELEASE_RESET).Times(1);

		EXPECT_CALL(halmock, HAL_NVIC_SetPriority).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ).Times(1);

		EXPECT_CALL(halmock, HAL_JPEG_RegisterCallback(_, HAL_JPEG_MSPINIT_CB_ID, _)).Times(1);
		EXPECT_CALL(halmock, HAL_JPEG_Init).Times(1);

		EXPECT_CALL(halmock, HAL_JPEG_RegisterInfoReadyCallback).Times(1);
		EXPECT_CALL(halmock, HAL_JPEG_RegisterGetDataCallback).Times(1);
		EXPECT_CALL(halmock, HAL_JPEG_RegisterDataReadyCallback).Times(1);
		EXPECT_CALL(halmock, HAL_JPEG_RegisterCallback(_, HAL_JPEG_DECODE_CPLT_CB_ID, _)).Times(1);
		EXPECT_CALL(halmock, HAL_JPEG_RegisterCallback(_, HAL_JPEG_ERROR_CB_ID, _)).Times(1);
	}

	corejpeg.initialize();
}

TEST_F(CoreJPEGTest, decodeImage)
{
	EXPECT_CALL(modemock, decode).Times(1);

	corejpeg.decodeImage(filemock);
}

TEST_F(CoreJPEGTest, findSOIMarkerFileIsEmpty)
{
	EXPECT_CALL(filemock, size).WillOnce(Return(0));

	corejpeg.findSOIMarker(filemock, 0);
}

TEST_F(CoreJPEGTest, findSOIMarkerBase)
{
	auto start_index		  = 0;
	auto expected_frame_index = 0;

	EXPECT_CALL(filemock, size).WillRepeatedly(Return(1024));
	// EXPECT_CALL(filemock, seek(start_index, _)).Times(AnyNumber());
	EXPECT_CALL(filemock, seek).Times(AnyNumber());

	EXPECT_CALL(filemock, read(Matcher<uint8_t *>(_), _)).WillRepeatedly(Return(512));

	auto actual_frame_index = corejpeg.findSOIMarker(filemock, start_index);
	EXPECT_EQ(actual_frame_index, expected_frame_index);
}

TEST_F(CoreJPEGTest, findSOIMarkerPresent)
{
	auto start_index = 0;

	std::array<uint8_t, 1024> data {};
	std::fill(std::begin(data), std::end(data), 0);

	const auto expected_frame_index	  = size_t {218};
	data.at(expected_frame_index)	  = (jpeg::JPEG_SOI_MARKER >> 8) & 0xFF;
	data.at(expected_frame_index + 1) = jpeg::JPEG_SOI_MARKER & 0xFF;

	EXPECT_CALL(filemock, size).Times(1).WillRepeatedly(Return(1024));
	EXPECT_CALL(filemock, seek(start_index, _)).Times(1);

	auto read_bytes = 512;
	EXPECT_CALL(filemock, read(Matcher<uint8_t *>(_), _))
		.Times(1)
		.WillRepeatedly(
			DoAll(SetArrayArgument<0>(std::begin(data), std::begin(data) + read_bytes), Return(read_bytes)));

	auto actual_frame_index = corejpeg.findSOIMarker(filemock, start_index);
	EXPECT_EQ(actual_frame_index, expected_frame_index);
}

TEST_F(CoreJPEGTest, findSOIMarkerAbsent)
{
	auto start_index = 0;

	std::array<uint8_t, 1024> data {};
	std::fill(std::begin(data), std::end(data), 0);

	const auto expected_frame_index = size_t {0};

	EXPECT_CALL(filemock, size).Times(AnyNumber()).WillRepeatedly(Return(1024));
	EXPECT_CALL(filemock, seek).Times(AnyNumber());

	auto read_bytes = 256;
	EXPECT_CALL(filemock, read(Matcher<uint8_t *>(_), _))
		.Times(4)
		.WillOnce(DoAll(SetArrayArgument<0>(std::begin(data) + 0 * read_bytes, std::begin(data) + 1 * read_bytes),
						Return(read_bytes)))
		.WillOnce(DoAll(SetArrayArgument<0>(std::begin(data) + 1 * read_bytes, std::begin(data) + 2 * read_bytes),
						Return(read_bytes)))
		.WillOnce(DoAll(SetArrayArgument<0>(std::begin(data) + 2 * read_bytes, std::begin(data) + 3 * read_bytes),
						Return(read_bytes)))
		.WillOnce(DoAll(SetArrayArgument<0>(std::begin(data) + 3 * read_bytes, std::begin(data) + 4 * read_bytes),
						Return(read_bytes)));

	auto actual_frame_index = corejpeg.findSOIMarker(filemock, start_index);
	EXPECT_EQ(actual_frame_index, expected_frame_index);
}

TEST_F(CoreJPEGTest, findSOIMarkerPresentInLastPacket)
{
	auto start_index = 0;

	std::array<uint8_t, 1024> data {};
	std::fill(std::begin(data), std::end(data), 0);

	const auto expected_frame_index	  = size_t {1001};
	data.at(expected_frame_index)	  = (jpeg::JPEG_SOI_MARKER >> 8) & 0xFF;
	data.at(expected_frame_index + 1) = jpeg::JPEG_SOI_MARKER & 0xFF;

	EXPECT_CALL(filemock, size).Times(AnyNumber()).WillRepeatedly(Return(1024));
	EXPECT_CALL(filemock, seek).Times(AnyNumber());

	auto read_bytes = 256;
	EXPECT_CALL(filemock, read(Matcher<uint8_t *>(_), _))
		.Times(4)
		.WillOnce(DoAll(SetArrayArgument<0>(std::begin(data) + 0 * read_bytes, std::begin(data) + 1 * read_bytes),
						Return(read_bytes)))
		.WillOnce(DoAll(SetArrayArgument<0>(std::begin(data) + 1 * read_bytes, std::begin(data) + 2 * read_bytes),
						Return(read_bytes)))
		.WillOnce(DoAll(SetArrayArgument<0>(std::begin(data) + 2 * read_bytes, std::begin(data) + 3 * read_bytes),
						Return(read_bytes)))
		.WillOnce(DoAll(SetArrayArgument<0>(std::begin(data) + 3 * read_bytes, std::begin(data) + 4 * read_bytes),
						Return(read_bytes)));

	auto actual_frame_index = corejpeg.findSOIMarker(filemock, start_index);
	EXPECT_EQ(actual_frame_index, expected_frame_index);
}

TEST_F(CoreJPEGTest, findSOIMarkerPresentInLastPacketStartingIndexWithOffset)
{
	auto start_index = 512;

	std::array<uint8_t, 1024> data {};
	std::fill(std::begin(data), std::end(data), 0);

	const auto expected_frame_index	  = size_t {1001};
	data.at(expected_frame_index)	  = (jpeg::JPEG_SOI_MARKER >> 8) & 0xFF;
	data.at(expected_frame_index + 1) = jpeg::JPEG_SOI_MARKER & 0xFF;

	EXPECT_CALL(filemock, size).Times(AnyNumber()).WillRepeatedly(Return(1024));
	EXPECT_CALL(filemock, seek).Times(AnyNumber());

	auto read_bytes = 256;
	EXPECT_CALL(filemock, read(Matcher<uint8_t *>(_), _))
		.Times(2)
		.WillOnce(DoAll(SetArrayArgument<0>(std::begin(data) + start_index + 0 * read_bytes,
											std::begin(data) + start_index + 1 * read_bytes),
						Return(read_bytes)))
		.WillOnce(DoAll(SetArrayArgument<0>(std::begin(data) + start_index + 1 * read_bytes,
											std::begin(data) + start_index + 2 * read_bytes),
						Return(read_bytes)));

	auto actual_frame_index = corejpeg.findSOIMarker(filemock, start_index);
	EXPECT_EQ(actual_frame_index, expected_frame_index);
}
