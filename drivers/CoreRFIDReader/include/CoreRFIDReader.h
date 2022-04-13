// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <span>

#include "events/EventQueue.h"
#include "rtos/Thread.h"

#include "interface/drivers/BufferedSerial.h"
#include "interface/drivers/RFIDReader.h"

namespace leka {

namespace rfid {

	struct RFIDProtocol {
		const uint8_t id;
		const std::byte gain;
		const std::byte modulation;
		[[nodiscard]] constexpr auto gain_modulation_values() const -> uint8_t
		{
			return std::to_integer<uint8_t>(gain | modulation);
		}
	};

	static constexpr size_t max_tx_length = 16;
	static constexpr size_t max_rx_length = 32;

	namespace settings {

		static constexpr uint8_t default_rx_tx_speed				 = 0x00;
		static constexpr uint8_t arc_b								 = 0x68;   // Analog Register Configuration
		static constexpr uint8_t flag_increment						 = 0x01;
		static constexpr uint8_t acr_b_index_for_gain_and_modulation = 0x01;

		namespace idle_tag_detection {

			static constexpr uint8_t tag_detection_command				   = 0x07;
			static constexpr uint8_t wu_source							   = 0x0A;
			static constexpr uint8_t length								   = 0x0E;
			static constexpr std::array<uint8_t, 2> enter_control		   = {0x21, 0x00};
			static constexpr std::array<uint8_t, 2> wu_control			   = {0x79, 0x01};
			static constexpr std::array<uint8_t, 2> leave_control		   = {0x18, 0x00};
			static constexpr uint8_t wu_periode							   = 0x20;
			static constexpr uint8_t oscillator_start					   = 0x60;
			static constexpr uint8_t digital_to_analog_start			   = 0x60;
			static constexpr std::array<uint8_t, 2> digital_to_analog_data = {0x70, 0x80};
			static constexpr uint8_t swing_count						   = 0x3F;
			static constexpr uint8_t max_sleep							   = 0x08;

		}	// namespace idle_tag_detection

	}	// namespace settings

	namespace tag_answer {

		static constexpr size_t heading_size = 2;
		static constexpr size_t flag_size	 = 3;

	}	// namespace tag_answer

	namespace expected_answer_size {

		static constexpr size_t idn						   = 17;
		static constexpr size_t tag_detection			   = 3;
		static constexpr size_t set_baudrate			   = 1;
		static constexpr size_t set_communication_protocol = 2;

	}	// namespace expected_answer_size

	namespace protocol {

		static constexpr RFIDProtocol iso15693 = {
			.id = 0x01, .gain = std::byte {0x00}, .modulation = std::byte {0xD0}};	 // gain = 34 dB, modulation = 95%
		static constexpr RFIDProtocol iso14443A = {
			.id = 0x02, .gain = std::byte {0x00}, .modulation = std::byte {0xD0}};	 // gain = 34 dB, modulation = 95%
		static constexpr RFIDProtocol iso14443B = {
			.id = 0x03, .gain = std::byte {0x00}, .modulation = std::byte {0x20}};	 // gain = 34 dB, modulation = 17%
		static constexpr RFIDProtocol iso18092 = {
			.id = 0x04, .gain = std::byte {0x00}, .modulation = std::byte {0x20}};	 // gain = 34 dB, modulation = 17%

	};	 // namespace protocol

	namespace status {

		static constexpr uint8_t communication_succeed		  = 0x80;
		static constexpr uint8_t error_time_out				  = 0x87;
		static constexpr std::array<uint8_t, 2> idn_success	  = {0x00, 0x0F};
		static constexpr std::array<uint8_t, 2> setup_success = {0x00, 0x00};
		static constexpr std::array<uint8_t, 2> tag_detection_callback {0x01, 0x02};

	}	// namespace status

	namespace command {

		static constexpr uint8_t send_receive = 0x04;

		namespace idn {
			static constexpr uint8_t id		= 0x01;
			static constexpr uint8_t length = 0x00;
		}	// namespace idn

		namespace set_baudrate {
			static constexpr uint8_t id		= 0x0A;
			static constexpr uint8_t length = 0x01;
		}	// namespace set_baudrate

		namespace set_protocol {
			static constexpr uint8_t id		= 0x02;
			static constexpr uint8_t length = 0x02;
		}	// namespace set_protocol

		namespace set_gain_and_modulation {
			static constexpr uint8_t id		= 0x09;
			static constexpr uint8_t length = 0x04;
		}	// namespace set_gain_and_modulation

		namespace frame {
			static constexpr std::array<uint8_t, 16> set_mode_tag_detection {
				rfid::settings::idle_tag_detection::tag_detection_command,
				rfid::settings::idle_tag_detection::length,
				rfid::settings::idle_tag_detection::wu_source,
				rfid::settings::idle_tag_detection::enter_control[0],
				rfid::settings::idle_tag_detection::enter_control[1],
				rfid::settings::idle_tag_detection::wu_control[0],
				rfid::settings::idle_tag_detection::wu_control[1],
				rfid::settings::idle_tag_detection::leave_control[0],
				rfid::settings::idle_tag_detection::leave_control[1],
				rfid::settings::idle_tag_detection::wu_periode,
				rfid::settings::idle_tag_detection::oscillator_start,
				rfid::settings::idle_tag_detection::digital_to_analog_start,
				rfid::settings::idle_tag_detection::digital_to_analog_data[0],
				rfid::settings::idle_tag_detection::digital_to_analog_data[1],
				rfid::settings::idle_tag_detection::swing_count,
				rfid::settings::idle_tag_detection::max_sleep};

			static constexpr std::array<uint8_t, 2> idn {
				rfid::command::idn::id,
				rfid::command::idn::length,
			};

			static constexpr std::array<uint8_t, 3> set_baudrate {rfid::command::set_baudrate::id,
																  rfid::command::set_baudrate::length};

			static constexpr std::array<uint8_t, 4> set_protocol_iso14443 {
				rfid::command::set_protocol::id, rfid::command::set_protocol::length, rfid::protocol::iso14443A.id,
				rfid::settings::default_rx_tx_speed};

			static constexpr std::array<uint8_t, 6> set_gain_and_modulation {
				rfid::command::set_gain_and_modulation::id,
				rfid::command::set_gain_and_modulation::length,
				rfid::settings::arc_b,
				rfid::settings::flag_increment,
				rfid::settings::acr_b_index_for_gain_and_modulation,
				rfid::protocol::iso14443A.gain_modulation_values()};

		}	// namespace frame

	}	// namespace command

}	// namespace rfid

class CoreRFIDReader : public interface::RFIDReader
{
  public:
	explicit CoreRFIDReader(interface::BufferedSerial &serial, rtos::Thread &thread, events::EventQueue &event_queue)
		: _serial(serial), _thread(thread), _event_queue(event_queue) {};

	void init() final;

	void registerTagAvailableCallback(tagAvailableCallback callback) final;
	void onDataAvailable() final;

	// auto getIDN(std::array<uint8_t, rfid::expected_answer_size::idn> &) -> bool final;
	auto setBaudrate(uint8_t baudrate) -> bool final;

	auto setCommunicationProtocol(rfid::Protocol protocol) -> bool final;

	void sendCommandToTag(std::span<uint8_t> cmd) final;
	auto receiveDataFromTag(std::span<uint8_t> data) -> bool final;

	void setModeTagDetection() final;

	auto checkForTagDetection() -> bool final;

  private:
	void registerCallback();
	void onCallback();

	auto receiveTagDetectionCallback() -> bool;

	auto writeConfiguration(std::span<uint8_t> conf) -> size_t;

	// void askRFIDForIDN();
	// auto didIDNIsCorrect() -> bool;

	auto didSetBaudrateSucceed(uint8_t baudrate) -> bool;

	auto setProtocolISO14443A() -> bool;
	auto setGainAndModulationISO14443A() -> bool;

	auto didsetCommunicationProtocolSucceed() -> bool;
	void read();

	auto formatCommand(std::span<uint8_t> cmd) -> size_t;

	auto DataFromTagIsCorrect(size_t sizeTagData) -> bool;
	void copyTagDataToSpan(std::span<uint8_t> data);

	tagAvailableCallback _tagAvailableCallback;
	bool _tagWasDetected {false};

	interface::BufferedSerial &_serial;
	rtos::Thread &_thread;
	events::EventQueue &_event_queue;

	size_t _anwser_size {0};
	std::array<uint8_t, rfid::max_tx_length> _tx_buf {};
	std::array<uint8_t, rfid::max_rx_length> _rx_buf {};

	rfid::Tag _tag {};
};

}	// namespace leka
