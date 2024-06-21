// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEServiceMagicCard.h"
#include <array>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace leka;

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
	auto magic_card_id		 = uint16_t {0x1234};
	auto magic_card_language = uint8_t {0x56};

	auto rfid_tag	= rfid::Tag {.data = {
									 0x4C,
									 0x45,
									 0x4B,
									 0x41,
									 utils::memory::getHighByte(magic_card_id),
									 utils::memory::getLowByte(magic_card_id),
									 magic_card_language,
								 }};
	auto magic_card = MagicCard {rfid_tag};

	std::array<uint8_t, 3> expected_raw_data {0x12, 0x34, 0x56};
	std::array<uint8_t, 3> actual_raw_data {};

	auto spy_callback = [&actual_raw_data](const BLEServiceMagicCard::data_to_send_handle_t &handle) {
		for (auto i = 0; i < std::size(actual_raw_data); i++) {
			actual_raw_data.at(i) = std::get<1>(handle)[i];
		}
	};
	service_magic_card.onDataReadyToSend(spy_callback);

	service_magic_card.setMagicCard(magic_card);
	EXPECT_EQ(actual_raw_data, expected_raw_data);
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
