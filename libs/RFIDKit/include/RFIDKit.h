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

	void init()
	{
		using namespace rfid::sm;

		static auto tag_detected_callback = [this]() { state_machine.process_event(event::tag_detected {}); };

		_rfid_reader.registerOnTagDetectedCallback(tag_detected_callback);
		registerMagicCard();

		_rfid_reader.init();
		_rfid_reader.setTagDetectionMode();
	}

	void registerMagicCard()
	{
		auto on_magic_card_readable_callback = [this]() {
			rfid::Tag tag = _rfid_reader.getTag();
			if (isTagSignatureValid(tag)) {
				_card = MagicCard {tag.data[5]};

				if (_on_tag_available_callback != nullptr) {
					_on_tag_available_callback(_card);
				}
			}
		};
		_rfid_reader.registerOnTagValidCallback(on_magic_card_readable_callback);
	}

	auto isTagSignatureValid(rfid::Tag tag) -> bool
	{
		return std::equal(std::begin(leka_tag_header), std::end(leka_tag_header), std::begin(tag.data));
	}

	void onTagActivated(std::function<void(MagicCard &_card)> callback) { _on_tag_available_callback = callback; }

  private:
	interface::RFIDReader &_rfid_reader;
	MagicCard _card = MagicCard::none;
	std::function<void(MagicCard &)> _on_tag_available_callback;
	boost::sml::sm<rfid::ISO14443A> state_machine {_rfid_reader};

	static constexpr std::array<uint8_t, 4> leka_tag_header = {0x4C, 0x45, 0x4B, 0x41};
};

}	// namespace leka
