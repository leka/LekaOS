// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <boost/sml.hpp>
#include <cstddef>
#include <span>

#include "events/EventQueue.h"

#include "LogKit.h"
#include "MagicCard.h"
#include "StateMachineISO14443A.h"
#include "Thread.h"
#include "interface/drivers/RFIDReader.h"

namespace leka {

class RFIDKit
{
  public:
	boost::sml::sm<rfid::StateMachineISO14443A> state_machine {_rfid_reader};
	explicit RFIDKit(interface::RFIDReader &rfid_reader) : _rfid_reader(rfid_reader) {};

	void init()
	{
		using namespace rfid::sm;

		static auto getTagDataCallback = [this]() {
			raise(event::tag_detected {});
			log_debug("Step1");
		};
		_rfid_reader.registerTagAvailableCallback(getTagDataCallback);
		log_debug("kit_init");
		_rfid_reader.init();
		registerEvents();
	}

	void registerEvents()
	{
		using namespace rfid::sm;

		auto on_atqa_received = [this]() { raise(event::atqa_received {}); };
		_rfid_reader.registerOnATQARequestCallback(on_atqa_received);

		auto on_register_received = [this]() { raise(event::register_received {}); };
		_rfid_reader.registerOnRegisterCallback(on_register_received);

		auto on_tag_valid = [this]() {
			raise(event::tag_valid {});
			registerMagicCard();
		};
		_rfid_reader.registerOnTagValidCallback(on_tag_valid);

		log_debug("events_registered");
	};

	void registerMagicCard()
	{
		_card = MagicCard {_tag.data[5]};
		_on_tag_available_callback(_card);
	}

	void raise(auto event)
	{
		_eq.call([this, &event] { state_machine.process_event(event); });
	}

	void onTagActivated(std::function<void(MagicCard &_card)> callback) { _on_tag_available_callback = callback; }

  private:
	interface::RFIDReader &_rfid_reader;
	rfid::Tag _tag {};
	MagicCard _card = MagicCard::none;
	std::function<void(MagicCard &)> _on_tag_available_callback;
	rtos::Thread _thread {};
	events::EventQueue _eq {};
};

}	// namespace leka
