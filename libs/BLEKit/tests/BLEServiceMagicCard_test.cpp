// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEServiceMagicCard.h"
#include <array>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace leka;

using ::testing::MockFunction;

class BLEServiceMagicCardTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	BLEServiceMagicCard service_magic_card {};

	std::array<const char, 256> default_expected_file_path {};

	BLEServiceMagicCard::data_received_handle_t data_received_handle {};
	BLEServiceMagicCard::data_requested_handle_t data_requested_handle {};

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

			service_magic_card.onDataReceived(data_received_handle);
		}

		const auto remaining_bytes = static_cast<int>(std::size(data) % packet_size);

		std::fill(std::begin(packet_array), std::end(packet_array), 0);
		std::copy(std::end(data) - remaining_bytes, std::end(data), packet_array.begin());

		data_received_handle.len	= remaining_bytes;
		data_received_handle.offset = expected_complete_packet * packet_size;
		data_received_handle.data	= packet_array.data();

		service_magic_card.onDataReceived(data_received_handle);
	}
};

TEST_F(BLEServiceMagicCardTest, initialisation)
{
	EXPECT_NE(&service_magic_card, nullptr);
}

TEST_F(BLEServiceMagicCardTest, setMagicCard)
{
	auto expected_id	   = uint16_t {0x1234};
	auto expected_language = uint8_t {0x56};

	auto rfid_tag	= rfid::Tag {.data = {
									 0x4C,
									 0x45,
									 0x4B,
									 0x41,
									 utils::memory::getHighByte(expected_id),
									 utils::memory::getLowByte(expected_id),
									 expected_language,
								 }};
	auto magic_card = MagicCard {rfid_tag};

	auto spy_callback = [&expected_id, &expected_language](const BLEServiceMagicCard::data_to_send_handle_t &handle) {
		auto data = std::get<1>(handle);

		auto is_id		 = std::size(data) == 2;
		auto is_language = std::size(data) == 1;

		if (is_id) {
			auto actual_id = utils::memory::combineBytes({.high = data[0], .low = data[1]});
			EXPECT_EQ(actual_id, expected_id);
		}
		if (is_language) {
			auto actual_language = data[0];
			EXPECT_EQ(actual_language, expected_language);
		}
	};
	service_magic_card.onDataReadyToSend(spy_callback);

	service_magic_card.setMagicCard(magic_card);
}

TEST_F(BLEServiceMagicCardTest, onDataReceived)
{
	auto dummy_params = BLEServiceMagicCard::data_received_handle_t {};
	service_magic_card.onDataReceived(dummy_params);

	// nothing expected
}

TEST_F(BLEServiceMagicCardTest, onDataRequested)
{
	auto dummy_params = BLEServiceMagicCard::data_requested_handle_t {};
	service_magic_card.onDataRequested(dummy_params);

	// nothing expected
}
