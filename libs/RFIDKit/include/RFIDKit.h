// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ISO14443A.h"
#include "MagicCard.h"
#include "interface/drivers/RFIDReader.h"

namespace leka {

class RFIDKit
{
  public:
	explicit RFIDKit(interface::RFIDReader &rfid_reader) : _rfid_reader(rfid_reader) {};

	void init();
	void registerMagicCard();
	[[nodiscard]] auto isTagSignatureValid(rfid::Tag tag) const -> bool;
	void onTagActivated(std::function<void(const MagicCard &_card)> const &callback);

	[[nodiscard]] auto getCallback() const -> const std::function<void(const MagicCard)> &;
	[[nodiscard]] auto getLastMagicCardActivated() const -> const MagicCard &;

  private:
	interface::RFIDReader &_rfid_reader;
	MagicCard _card = MagicCard::none;
	std::function<void(const MagicCard)> _on_tag_available_callback;
	boost::sml::sm<rfid::ISO14443A> state_machine {_rfid_reader};

	static constexpr std::array<uint8_t, 4> leka_tag_header = {0x4C, 0x45, 0x4B, 0x41};
};

}	// namespace leka
