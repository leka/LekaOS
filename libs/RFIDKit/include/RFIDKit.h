// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <boost/sml.hpp>
#include <cstddef>
#include <span>

#include "MagicCard.h"
#include "RFIDStateMachine.h"
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

	auto isTagSignatureValid() -> bool final;

	void sendREQA() final;
	void sendReadRegister0() final;
	void sendReadRegister4() final;

	auto receiveATQA() -> bool final;
	auto receiveReadTagData() -> bool final;

	void registerMagicCard() final;

  private:
	auto computeCRC(uint8_t const *data) const -> std::array<uint8_t, 2>;

	interface::RFIDReader &_rfid_reader;
	rfid::Tag _tag {};
	MagicCard _card = MagicCard::none;
	std::function<void(MagicCard &)> _on_tag_available_callback;
	state _state = state::WAITING_FOR_TAG;
};

}	// namespace leka
