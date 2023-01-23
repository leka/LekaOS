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
using ::testing::SaveArg;
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
	std::function<void(JPEG_HandleTypeDef *)> hal_jpeg_mspinit_callback = [](JPEG_HandleTypeDef *) {};
	std::function<void(JPEG_HandleTypeDef *, JPEG_ConfTypeDef *)> hal_jpeg_info_ready_callback =
		[](JPEG_HandleTypeDef *, JPEG_ConfTypeDef *) {};
	std::function<void(JPEG_HandleTypeDef *, uint32_t)> hal_jpeg_get_data_callback = [](JPEG_HandleTypeDef *,
																						uint32_t) {};
	std::function<void(JPEG_HandleTypeDef *, uint8_t *, uint32_t)> hal_jpeg_data_ready_callback =
		[](JPEG_HandleTypeDef *, uint8_t *, uint32_t) {};
	std::function<void(JPEG_HandleTypeDef *)> hal_jpeg_decode_complete_callback = [](JPEG_HandleTypeDef *) {};
	std::function<void(JPEG_HandleTypeDef *)> hal_jpeg_error_callback			= [](JPEG_HandleTypeDef *) {};

	{
		InSequence seq;

		EXPECT_CALL(halmock, HAL_RCC_JPEG_CLK_ENABLE).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_JPEG_FORCE_RESET).Times(1);
		EXPECT_CALL(halmock, HAL_RCC_JPEG_RELEASE_RESET).Times(1);

		EXPECT_CALL(halmock, HAL_NVIC_SetPriority).Times(1);
		EXPECT_CALL(halmock, HAL_NVIC_EnableIRQ).Times(1);

		EXPECT_CALL(halmock, HAL_JPEG_RegisterCallback(_, HAL_JPEG_MSPINIT_CB_ID, _))
			.WillOnce(DoAll(SaveArg<2>(&hal_jpeg_mspinit_callback), Return(HAL_StatusTypeDef::HAL_OK)));
		EXPECT_CALL(halmock, HAL_JPEG_Init).Times(1);

		EXPECT_CALL(halmock, HAL_JPEG_RegisterInfoReadyCallback)
			.WillOnce(DoAll(SaveArg<1>(&hal_jpeg_info_ready_callback), Return(HAL_StatusTypeDef::HAL_OK)));
		EXPECT_CALL(halmock, HAL_JPEG_RegisterGetDataCallback)
			.WillOnce(DoAll(SaveArg<1>(&hal_jpeg_get_data_callback), Return(HAL_StatusTypeDef::HAL_OK)));
		EXPECT_CALL(halmock, HAL_JPEG_RegisterDataReadyCallback)
			.WillOnce(DoAll(SaveArg<1>(&hal_jpeg_data_ready_callback), Return(HAL_StatusTypeDef::HAL_OK)));
		EXPECT_CALL(halmock, HAL_JPEG_RegisterCallback(_, HAL_JPEG_DECODE_CPLT_CB_ID, _))
			.WillOnce(DoAll(SaveArg<2>(&hal_jpeg_decode_complete_callback), Return(HAL_StatusTypeDef::HAL_OK)));
		EXPECT_CALL(halmock, HAL_JPEG_RegisterCallback(_, HAL_JPEG_ERROR_CB_ID, _))
			.WillOnce(DoAll(SaveArg<2>(&hal_jpeg_error_callback), Return(HAL_StatusTypeDef::HAL_OK)));
	}

	corejpeg.initialize();

	// ? - Verification of callback registered

	auto jpeg_handle = JPEG_HandleTypeDef {};

	EXPECT_CALL(modemock, onMspInitCallback(&jpeg_handle));
	hal_jpeg_mspinit_callback(&jpeg_handle);

	auto jpeg_conf = JPEG_ConfTypeDef {};
	EXPECT_CALL(modemock, onInfoReadyCallback(&jpeg_handle, &jpeg_conf));
	hal_jpeg_info_ready_callback(&jpeg_handle, &jpeg_conf);

	auto any_datasize = uint8_t {42};
	EXPECT_CALL(modemock, onDataAvailableCallback(&jpeg_handle, any_datasize));
	hal_jpeg_get_data_callback(&jpeg_handle, any_datasize);

	auto output_data = std::array<uint8_t, 8> {};
	EXPECT_CALL(modemock, onDataReadyCallback(&jpeg_handle, output_data.data(), any_datasize));
	hal_jpeg_data_ready_callback(&jpeg_handle, output_data.data(), any_datasize);

	EXPECT_CALL(modemock, onDecodeCompleteCallback(&jpeg_handle));
	hal_jpeg_decode_complete_callback(&jpeg_handle);

	EXPECT_CALL(modemock, onErrorCallback(&jpeg_handle));
	hal_jpeg_error_callback(&jpeg_handle);
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

	const auto expected_frame_index	  = std::size_t {218};
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

	const auto expected_frame_index = std::size_t {0};

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

	const auto expected_frame_index	  = std::size_t {1001};
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

	const auto expected_frame_index	  = std::size_t {1001};
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
