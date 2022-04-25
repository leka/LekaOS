// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstddef>
#include <span>

#include "MagicCard.h"
#include "interface/drivers/RFIDReader.h"

namespace leka {

enum class state : uint8_t
{

	WAITING_FOR_TAG				   = 0x00,
	TAG_COMMUNICATION_PROTOCOL_SET = 0x01,
	WAIT_FOR_ATQA_RESPONSE		   = 0x02,
	TAG_IDENTIFIED				   = 0x03,
	TAG_AVAILABLE				   = 0x04,

};

class RFIDKit : public interface::RFIDReader::ISO14443
{
  public:
	explicit RFIDKit(interface::RFIDReader &rfid_reader) : _rfid_reader(rfid_reader) {};

	void init() final;

	void runStateMachine() final;

	void onTagActivated(std::function<void(MagicCard &)> callback);

  private:
	void sendREQA();
	void sendReadRegister0();
	void sendReadRegister4();

	auto isTagSignatureValid() -> bool;

	auto receiveATQA() -> bool;
	auto receiveReadTagData() -> bool;

	auto computeCRC(uint8_t const *data) const -> std::array<uint8_t, 2>;

	interface::RFIDReader &_rfid_reader;
	rfid::Tag _tag {};
	MagicCard _card = MagicCard::none;
	std::function<void(MagicCard &)> _on_tag_available_callback;
	state _state = state::WAITING_FOR_TAG;
};

}	// namespace leka
