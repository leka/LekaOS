// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEServiceConfig.h"
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

class BLEServiceConfigTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	BLEServiceConfig service_config {};

	BLEServiceConfig::data_received_handle_t data_received_handle {};
	BLEServiceConfig::data_requested_handle_t data_requested_handle {};

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

			service_config.onDataReceived(data_received_handle);
		}

		const auto remaining_bytes = static_cast<int>(std::size(data) % packet_size);

		std::fill(std::begin(packet_array), std::end(packet_array), 0);
		std::copy(std::end(data) - remaining_bytes, std::end(data), packet_array.begin());

		data_received_handle.len	= remaining_bytes;
		data_received_handle.offset = expected_complete_packet * packet_size;
		data_received_handle.data	= packet_array.data();

		service_config.onDataReceived(data_received_handle);
	}
};

TEST_F(BLEServiceConfigTest, initialisation)
{
	EXPECT_NE(&service_config, nullptr);
}

TEST_F(BLEServiceConfigTest, setRobotName)
{
	auto name		   = std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize> {"Leka"};
	auto expected_name = std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize> {"Leka"};
	auto actual_name   = std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize> {};

	auto spy_callback = [&actual_name](const BLEServiceConfig::data_to_send_handle_t &handle) {
		for (auto i = 0; i < std::size(actual_name); i++) {
			actual_name.at(i) = std::get<1>(handle)[i];
		}
	};
	service_config.onDataReadyToSend(spy_callback);

	service_config.setRobotName(name);
	EXPECT_EQ(actual_name, expected_name);
}

TEST_F(BLEServiceConfigTest, setRobotNameOversize)
{
	auto name = std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize + 1> {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	};
	auto expected_name = std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize> {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
	};
	auto actual_name = std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize> {};

	auto spy_callback = [&actual_name](const BLEServiceConfig::data_to_send_handle_t &handle) {
		for (auto i = 0; i < std::size(actual_name); i++) {
			actual_name.at(i) = std::get<1>(handle)[i];
		}
	};
	service_config.onDataReadyToSend(spy_callback);

	service_config.setRobotName(name);
	EXPECT_EQ(actual_name, expected_name);
}

TEST_F(BLEServiceConfigTest, onRobotNameUpdated)
{
	auto name		   = std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize> {"Leka"};
	auto expected_name = std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize> {"Leka"};

	MockFunction<void(std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize>)> mock_callback {};
	service_config.onRobotNameUpdated(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call(compareArray(expected_name))).Times(1);
	onDataReceivedProcess(name);
}

TEST_F(BLEServiceConfigTest, onRobotNameUpdatedOversize)
{
	auto name = std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize + 1> {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	};

	MockFunction<void(std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize>)> mock_callback {};
	service_config.onRobotNameUpdated(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(0);
	onDataReceivedProcess(name);
}

TEST_F(BLEServiceConfigTest, onRobotNameUpdatedUnset)
{
	auto name = std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize> {"Leka"};

	onDataReceivedProcess(name);

	// nothing expected
}

TEST_F(BLEServiceConfigTest, onRobotNameUpdatedNotSameHandle)
{
	auto name = std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize> {"Leka"};

	data_received_handle.handle = 0xFFFF;

	MockFunction<void(std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize>)> mock_callback {};
	service_config.onRobotNameUpdated(mock_callback.AsStdFunction());

	EXPECT_CALL(mock_callback, Call).Times(0);
	onDataReceivedProcess(name);
}

TEST_F(BLEServiceConfigTest, onDataRequested)
{
	service_config.onDataRequested(data_requested_handle);

	// nothing expected
}
