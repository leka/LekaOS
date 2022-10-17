// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEServiceFileExchange.h"
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

class BLEServiceFileExchangeTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	BLEServiceFileExchange service_file_exchange {};

	std::array<const char, 256> default_expected_file_path {};

	BLEServiceFileExchange::data_received_handle_t data_received_handle {};
	BLEServiceFileExchange::data_requested_handle_t data_requested_handle {};

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

			service_file_exchange.onDataReceived(data_received_handle);
		}

		const auto remaining_bytes = static_cast<int>(std::size(data) % packet_size);

		std::fill(std::begin(packet_array), std::end(packet_array), 0);
		std::copy(std::end(data) - remaining_bytes, std::end(data), packet_array.begin());

		data_received_handle.len	= remaining_bytes;
		data_received_handle.offset = expected_complete_packet * packet_size;
		data_received_handle.data	= packet_array.data();

		service_file_exchange.onDataReceived(data_received_handle);
	}
};

TEST_F(BLEServiceFileExchangeTest, initialisation)
{
	EXPECT_NE(&service_file_exchange, nullptr);
}

TEST_F(BLEServiceFileExchangeTest, setFileExchangeState)
{
	uint8_t actual_charging_status {};

	auto spy_callback = [&actual_charging_status](const BLEServiceFileExchange::data_to_send_handle_t &handle) {
		actual_charging_status = std::get<1>(handle)[0];
	};

	service_file_exchange.onDataReadyToSend(spy_callback);

	service_file_exchange.setFileExchangeState(true);
	EXPECT_TRUE(actual_charging_status);

	service_file_exchange.setFileExchangeState(false);
	EXPECT_FALSE(actual_charging_status);
}

TEST_F(BLEServiceFileExchangeTest, getFileExchangeStateDefault)
{
	auto expected_is_file_exchange_state = false;

	auto actual_is_file_exchange_state = service_file_exchange.getFileExchangeState();

	EXPECT_EQ(actual_is_file_exchange_state, expected_is_file_exchange_state);
}

TEST_F(BLEServiceFileExchangeTest, getFileExchangeStateTrue)
{
	auto expected_is_file_exchange_state = true;

	std::array<uint8_t, 1> expected_is_file_exchange_state_data = {
		static_cast<uint8_t>(expected_is_file_exchange_state)};

	onDataReceivedProcess(expected_is_file_exchange_state_data);

	auto actual_is_file_exchange_state = service_file_exchange.getFileExchangeState();

	EXPECT_EQ(actual_is_file_exchange_state, expected_is_file_exchange_state);
}

TEST_F(BLEServiceFileExchangeTest, onSetFileExchangeStateCallback)
{
	bool sent_value		 = true;
	auto sent_value_data = static_cast<uint8_t>(sent_value);

	data_received_handle.data = &sent_value_data;

	testing::MockFunction<void(bool)> mock_callback {};
	service_file_exchange.onSetFileExchangeState(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call(sent_value));

	service_file_exchange.onDataReceived(data_received_handle);
}

TEST_F(BLEServiceFileExchangeTest, onSetFileExchangeStateCallbackNotSameHandle)
{
	bool sent_value		 = true;
	auto sent_value_data = static_cast<uint8_t>(sent_value);

	data_received_handle.data = &sent_value_data;

	testing::MockFunction<void(bool)> mock_callback {};
	service_file_exchange.onSetFileExchangeState(mock_callback.AsStdFunction());

	data_received_handle.handle = 0xFFFF;

	EXPECT_CALL(mock_callback, Call).Times(0);

	service_file_exchange.onDataReceived(data_received_handle);
}

TEST_F(BLEServiceFileExchangeTest, getFilePathAny)
{
	auto expected_file_path = std::to_array("/fs/some_file.txt");

	auto cast_expected_file_path = std::span<uint8_t>(reinterpret_cast<uint8_t *>(expected_file_path.begin()),
													  reinterpret_cast<uint8_t *>(expected_file_path.end()));

	testing::MockFunction<void(std::span<const char>)> mock_callback {};
	service_file_exchange.onFilePathReceived(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call(compareArray(expected_file_path))).Times(1);

	onDataReceivedProcess(cast_expected_file_path);
}

TEST_F(BLEServiceFileExchangeTest, getFilePathNotSameHandle)
{
	auto sent_file_path = std::to_array("/fs/some_file.txt");

	auto cast_sent_file_path = std::span<uint8_t>(reinterpret_cast<uint8_t *>(sent_file_path.begin()),
												  reinterpret_cast<uint8_t *>(sent_file_path.end()));

	data_received_handle.handle = 0xFFFF;

	testing::MockFunction<void(std::span<const char>)> mock_callback {};
	service_file_exchange.onFilePathReceived(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(0);

	onDataReceivedProcess(cast_sent_file_path);
}

TEST_F(BLEServiceFileExchangeTest, onFileDataReceivedCallback)
{
	auto expected_array = std::to_array<uint8_t>({0x2A, 0x2B, 0x2C, 0x2D, 0x2E});

	testing::MockFunction<void(std::span<const uint8_t>)> mock_callback {};
	service_file_exchange.onFileDataReceived(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call(compareArray(expected_array))).Times(1);

	onDataReceivedProcess(expected_array);
}

TEST_F(BLEServiceFileExchangeTest, onFileDataReceivedCallbackNotSameHandle)
{
	auto sent_data = std::to_array<uint8_t>({0x2A, 0x2B, 0x2C, 0x2D, 0x2E});

	testing::MockFunction<void(std::span<const uint8_t>)> mock_callback {};
	service_file_exchange.onFileDataReceived(mock_callback.AsStdFunction());

	data_received_handle.handle = 0xFFFF;

	EXPECT_CALL(mock_callback, Call).Times(0);

	onDataReceivedProcess(sent_data);
}

TEST_F(BLEServiceFileExchangeTest, onFileSHA256Requested)
{
	testing::MockFunction<void(std::span<const char>)> mock_callback {};
	service_file_exchange.onFileSHA256Requested(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(1);

	service_file_exchange.onDataRequested(data_requested_handle);
}

TEST_F(BLEServiceFileExchangeTest, onFileSHA256RequestedNotSameHandle)
{
	testing::MockFunction<void(std::span<const char>)> mock_callback {};
	service_file_exchange.onFileSHA256Requested(mock_callback.AsStdFunction());

	data_requested_handle.handle = 0xFFFF;

	EXPECT_CALL(mock_callback, Call).Times(0);

	service_file_exchange.onDataRequested(data_requested_handle);
}

TEST_F(BLEServiceFileExchangeTest, onFileSHA256RequestedtUnset)
{
	service_file_exchange.onFileSHA256Requested(nullptr);

	service_file_exchange.onDataRequested(data_requested_handle);
}

TEST_F(BLEServiceFileExchangeTest, setFileSHA256)
{
	std::array<uint8_t, 32> expected_sha256 {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	};
	std::array<uint8_t, 32> actual_sha256 {};

	auto spy_callback = [&actual_sha256](const BLEServiceFileExchange::data_to_send_handle_t &handle) {
		for (auto i = 0; i < std::size(actual_sha256); i++) {
			actual_sha256.at(i) = std::get<1>(handle)[i];
		}
	};

	service_file_exchange.onDataReadyToSend(spy_callback);

	service_file_exchange.setFileSHA256(expected_sha256);
	EXPECT_EQ(actual_sha256, expected_sha256);
}
