// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEServiceFileReception.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace leka;

using ::testing::MockFunction;

MATCHER_P(compareArray, expected_array, "")
{
	bool same_content = true;
	for (int i = 0; i < std::size(expected_array); i++) {
		same_content &= expected_array[i] == arg[i];
	}
	return same_content;
}

auto compareSpan(std::span<const char> actual_span, std::span<const char> expected_span) -> bool
{
	bool same_content = true;
	for (int i = 0; i < std::size(expected_span); i++) {
		same_content &= expected_span[i] == actual_span[i];
	}
	return same_content;
}

class BLEServiceFileReceptionTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	BLEServiceFileReception service_file_reception {};

	std::array<const char, 256> default_expected_file_path {};

	BLEServiceFileReception::data_received_handle_t data_received_handle {};
	BLEServiceFileReception::data_requested_handle_t data_requested_handle {};

	void onDataReceivedProcess(std::span<uint8_t> data)
	{
		const auto packet_size		  = 100;
		auto expected_complete_packet = static_cast<int>(std::size(data) / packet_size);

		std::array<uint8_t, packet_size> packet_array;

		for (auto packet = 0; packet < expected_complete_packet; packet++) {
			std::copy(std::begin(data) + (packet * packet_size), std::begin(data) + ((packet + 1) * packet_size),
					  packet_array.begin());

			data_received_handle.len	= packet_size;
			data_received_handle.offset = packet * packet_size;
			data_received_handle.data	= packet_array.data();

			service_file_reception.onDataReceived(data_received_handle);
		}

		const auto remaining_bytes = static_cast<int>(std::size(data) % packet_size);

		std::fill(std::begin(packet_array), std::end(packet_array), 0);
		std::copy(std::end(data) - remaining_bytes, std::end(data), packet_array.begin());

		data_received_handle.len	= remaining_bytes;
		data_received_handle.offset = expected_complete_packet * packet_size;
		data_received_handle.data	= packet_array.data();

		service_file_reception.onDataReceived(data_received_handle);
	}
};

TEST_F(BLEServiceFileReceptionTest, initialisation)
{
	EXPECT_NE(&service_file_reception, nullptr);
}

TEST_F(BLEServiceFileReceptionTest, getFilePathAny)
{
	auto expected_file_path = std::to_array("/fs/some_file.txt");

	auto cast_expected_file_path = std::span<uint8_t>(reinterpret_cast<uint8_t *>(expected_file_path.begin()),
													  reinterpret_cast<uint8_t *>(expected_file_path.end()));

	testing::MockFunction<void(std::span<const char>)> mock_callback {};
	service_file_reception.onFilePathReceived(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call(compareArray(expected_file_path))).Times(1);

	onDataReceivedProcess(cast_expected_file_path);
}

TEST_F(BLEServiceFileReceptionTest, getFilePathNotSameHandle)
{
	auto sent_file_path = std::to_array("/fs/some_file.txt");

	auto cast_sent_file_path = std::span<uint8_t>(reinterpret_cast<uint8_t *>(sent_file_path.begin()),
												  reinterpret_cast<uint8_t *>(sent_file_path.end()));

	data_received_handle.handle = 0xFFFF;

	testing::MockFunction<void(std::span<const char>)> mock_callback {};
	service_file_reception.onFilePathReceived(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(0);

	onDataReceivedProcess(cast_sent_file_path);
}

TEST_F(BLEServiceFileReceptionTest, onFileDataReceivedCallback)
{
	auto expected_array = std::to_array<uint8_t>({0x2A, 0x2B, 0x2C, 0x2D, 0x2E});

	testing::MockFunction<void(std::span<const uint8_t>)> mock_callback {};
	service_file_reception.onFileDataReceived(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call(compareArray(expected_array))).Times(1);

	onDataReceivedProcess(expected_array);
}

TEST_F(BLEServiceFileReceptionTest, onFileDataReceivedCallbackNotSameHandle)
{
	auto sent_data = std::to_array<uint8_t>({0x2A, 0x2B, 0x2C, 0x2D, 0x2E});

	testing::MockFunction<void(std::span<const uint8_t>)> mock_callback {};
	service_file_reception.onFileDataReceived(mock_callback.AsStdFunction());

	data_received_handle.handle = 0xFFFF;

	EXPECT_CALL(mock_callback, Call).Times(0);

	onDataReceivedProcess(sent_data);
}

TEST_F(BLEServiceFileReceptionTest, onDataRequested)
{
	service_file_reception.onDataRequested(data_requested_handle);

	// nothing expected
}
