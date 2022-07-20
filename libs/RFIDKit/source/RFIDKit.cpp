// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RFIDKit.h"

namespace leka {

void RFIDKit::init()
{
	using namespace rfid::sm;

	static auto tag_detected_callback = [this]() { state_machine.process_event(event::tag_detected {}); };

	_rfid_reader.registerOnTagDetectedCallback(tag_detected_callback);
	registerMagicCard();

	_rfid_reader.init();
	_rfid_reader.setTagDetectionMode();
}

void RFIDKit::registerMagicCard()
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

auto RFIDKit::isTagSignatureValid(rfid::Tag tag) -> bool
{
	return std::equal(std::begin(leka_tag_header), std::end(leka_tag_header), std::begin(tag.data));
}

void RFIDKit::onTagActivated(std::function<void(MagicCard &_card)> callback)
{
	_on_tag_available_callback = callback;
}

}	// namespace leka
