// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstddef>
#include <span>

#include "interface/drivers/RFIDReader.h"

namespace leka {

enum class state : uint8_t
{

	SENSOR_SLEEP				   = 0x00,
	TAG_COMMUNICATION_PROTOCOL_SET = 0x01,
	WAIT_FOR_ATQA_RESPONSE		   = 0x02,
	TAG_IDENTIFIED				   = 0x03,
	TAG_ACTIVATED				   = 0x04,

};

class RFIDKit : public interface::RFIDReader::ISO14443
{
  public:
	explicit RFIDKit(interface::RFIDReader &rfid_reader) : _rfid_reader(rfid_reader) {};

	void init() final;

	void getTagData() final;

	auto getTag() -> rfid::Tag final { return _tag; };

	void resetTag() { _tag.data = {}; };

	void onTagActivated(std::function<void(rfid::Tag &)> const &callback);

  private:
	template <size_t SIZE>
	void commandToArray(Command<SIZE> command, std::span<uint8_t> span) const
	{
		for (size_t i = 0; i < SIZE; ++i) {
			span.data()[i] = command.data.data()[i];
		}

		span.data()[SIZE] = static_cast<uint8_t>(command.flags);
	}

	void sendREQA();
	void sendReadRegister0();
	void sendReadRegister6();
	void sendWriteRegister(uint8_t registerToWrite, std::array<uint8_t, 4> data);

	auto receiveATQA() -> bool;
	auto receiveReadTagData() -> bool;
	void receiveWriteTagData();

	auto isTagSignatureValid() -> bool;

	void sendAuthentificate();
	void receiveAuthentificate();

	auto computeCRC(uint8_t const *data) const -> std::array<uint8_t, 2>;

	void getData(std::array<uint8_t, 16> &tag_data);

	interface::RFIDReader &_rfid_reader;
	rfid::Tag _tag {};
	std::function<void(rfid::Tag &)> _on_tag_activated_callback;
	state _state = state::SENSOR_SLEEP;
};

}	// namespace leka
