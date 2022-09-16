// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEServiceFileReception.h"
#include <array>

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

TEST_F(BLEServiceFileReceptionTest, onFileSHA256Requested)
{
	testing::MockFunction<void(std::span<const char>)> mock_callback {};
	service_file_reception.onFileSHA256Requested(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(1);

	service_file_reception.onDataRequested(data_requested_handle);
}

TEST_F(BLEServiceFileReceptionTest, onFileSHA256RequestedNotSameHandle)
{
	testing::MockFunction<void(std::span<const char>)> mock_callback {};
	service_file_reception.onFileSHA256Requested(mock_callback.AsStdFunction());

	data_requested_handle.handle = 0xFFFF;

	EXPECT_CALL(mock_callback, Call).Times(0);

	service_file_reception.onDataRequested(data_requested_handle);
}

TEST_F(BLEServiceFileReceptionTest, onFileSHA256RequestedtUnset)
{
	service_file_reception.onFileSHA256Requested(nullptr);

	service_file_reception.onDataRequested(data_requested_handle);
}

TEST_F(BLEServiceFileReceptionTest, setFileSHA256)
{
	std::array<uint8_t, 32> expected_sha256 {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	};
	std::array<uint8_t, 32> actual_sha256 {};

	auto spy_callback = [&actual_sha256](const BLEServiceFileReception::data_to_send_handle_t &handle) {
		for (auto i = 0; i < std::size(actual_sha256); i++) {
			actual_sha256.at(i) = std::get<1>(handle)[i];
		}
	};

	service_file_reception.onDataReadyToSend(spy_callback);

	service_file_reception.setFileSHA256(expected_sha256);
	EXPECT_EQ(actual_sha256, expected_sha256);
}
