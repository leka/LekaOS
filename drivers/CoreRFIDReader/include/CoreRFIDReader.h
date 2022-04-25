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

	inline constexpr size_t max_tx_length = 16;
	inline constexpr size_t max_rx_length = 32;

	namespace settings {

		inline constexpr uint8_t default_rx_tx_speed				 = 0x00;
		inline constexpr uint8_t arc_b								 = 0x68;   // Analog Register Configuration
		inline constexpr uint8_t flag_increment						 = 0x01;
		inline constexpr uint8_t acr_b_index_for_gain_and_modulation = 0x01;

		namespace idle_tag_detection {

			inline constexpr uint8_t tag_detection_command				   = 0x07;
			inline constexpr uint8_t wu_source							   = 0x0A;
			inline constexpr uint8_t length								   = 0x0E;
			inline constexpr std::array<uint8_t, 2> enter_control		   = {0x21, 0x00};
			inline constexpr std::array<uint8_t, 2> wu_control			   = {0x79, 0x01};
			inline constexpr std::array<uint8_t, 2> leave_control		   = {0x18, 0x00};
			inline constexpr uint8_t wu_periode							   = 0x20;
			inline constexpr uint8_t oscillator_start					   = 0x60;
			inline constexpr uint8_t digital_to_analog_start			   = 0x60;
			inline constexpr std::array<uint8_t, 2> digital_to_analog_data = {0x70, 0x80};
			inline constexpr uint8_t swing_count						   = 0x3F;
			inline constexpr uint8_t max_sleep							   = 0x08;

		}	// namespace idle_tag_detection

	}	// namespace settings

	namespace tag_answer {

		inline constexpr size_t heading_size = 2;
		inline constexpr size_t flag_size	 = 3;

	}	// namespace tag_answer

	namespace expected_answer_size {

		inline constexpr size_t idn						   = 17;
		inline constexpr size_t tag_detection			   = 3;
		inline constexpr size_t set_baudrate			   = 1;
		inline constexpr size_t set_communication_protocol = 2;

	}	// namespace expected_answer_size

	namespace protocol {

		inline constexpr RFIDProtocol iso15693 = {
			.id = 0x01, .gain = std::byte {0x00}, .modulation = std::byte {0xD0}};	 // gain = 34 dB, modulation = 95%
		inline constexpr RFIDProtocol iso14443A = {
			.id = 0x02, .gain = std::byte {0x00}, .modulation = std::byte {0xD0}};	 // gain = 34 dB, modulation = 95%
		inline constexpr RFIDProtocol iso14443B = {
			.id = 0x03, .gain = std::byte {0x00}, .modulation = std::byte {0x20}};	 // gain = 34 dB, modulation = 17%
		inline constexpr RFIDProtocol iso18092 = {
			.id = 0x04, .gain = std::byte {0x00}, .modulation = std::byte {0x20}};	 // gain = 34 dB, modulation = 17%

	};	 // namespace protocol

	namespace status {

		inline constexpr uint8_t communication_succeed		  = 0x80;
		inline constexpr uint8_t error_time_out				  = 0x87;
		inline constexpr std::array<uint8_t, 2> idn_success	  = {0x00, 0x0F};
		inline constexpr std::array<uint8_t, 2> setup_success = {0x00, 0x00};
		inline constexpr std::array<uint8_t, 2> tag_detection_callback {0x01, 0x02};

	}	// namespace status

	namespace command {

		inline constexpr uint8_t send_receive = 0x04;

		namespace idn {
			inline constexpr uint8_t id		= 0x01;
			inline constexpr uint8_t length = 0x00;
		}	// namespace idn

		namespace set_baudrate {
			inline constexpr uint8_t id		= 0x0A;
			inline constexpr uint8_t length = 0x01;
		}	// namespace set_baudrate

		namespace set_protocol {
			inline constexpr uint8_t id		= 0x02;
			inline constexpr uint8_t length = 0x02;
		}	// namespace set_protocol

		namespace set_gain_and_modulation {
			inline constexpr uint8_t id		= 0x09;
			inline constexpr uint8_t length = 0x04;
		}	// namespace set_gain_and_modulation

		namespace frame {
			inline constexpr std::array<uint8_t, 16> set_tag_detection_mode {
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
				rfid::settings::idle_tag_detection::max_sleep,
			};

			inline constexpr std::array<uint8_t, 2> idn {
				rfid::command::idn::id,
				rfid::command::idn::length,
			};

			inline constexpr std::array<uint8_t, 3> set_baudrate {
				rfid::command::set_baudrate::id,
				rfid::command::set_baudrate::length,
			};

			inline constexpr std::array<uint8_t, 4> set_protocol_iso14443 {
				rfid::command::set_protocol::id,
				rfid::command::set_protocol::length,
				rfid::protocol::iso14443A.id,
				rfid::settings::default_rx_tx_speed,
			};

			inline constexpr std::array<uint8_t, 6> set_gain_and_modulation {
				rfid::command::set_gain_and_modulation::id,
				rfid::command::set_gain_and_modulation::length,
				rfid::settings::arc_b,
				rfid::settings::flag_increment,
				rfid::settings::acr_b_index_for_gain_and_modulation,
				rfid::protocol::iso14443A.gain_modulation_values(),
			};

		}	// namespace frame

	}	// namespace command

}	// namespace rfid

class CoreRFIDReader : public interface::RFIDReader
{
  public:
	explicit CoreRFIDReader(interface::BufferedSerial &serial) : _serial(serial) {};

	void init() final;

	void registerTagAvailableCallback(tag_available_callback_t callback) final;
	void onDataAvailable() final;

	auto setBaudrate(uint8_t baudrate) -> bool final;

	auto setCommunicationProtocol(rfid::Protocol protocol) -> bool final;

	void sendCommandToTag(std::span<const uint8_t> cmd) final;
	auto receiveDataFromTag(std::span<uint8_t> data) -> bool final;

	void setTagDetectionMode() final;

	auto checkForTagDetection() -> bool final;

  private:
	void registerCallback();

	auto setBaudrateDidSucceed() -> bool;

	auto setProtocolISO14443A() -> bool;
	auto setGainAndModulationISO14443A() -> bool;

	auto didsetCommunicationProtocolSucceed() -> bool;
	void read();

	auto formatCommand(std::span<const uint8_t> cmd) -> size_t;

	auto DataFromTagIsCorrect(size_t sizeTagData) -> bool;
	void copyTagDataToSpan(std::span<uint8_t> data);

	tag_available_callback_t _tagAvailableCallback;

	interface::BufferedSerial &_serial;
	rtos::Thread _thread {};
	events::EventQueue _event_queue {};

	size_t _anwser_size {0};
	std::array<uint8_t, rfid::max_tx_length> _tx_buf {};
	std::array<uint8_t, rfid::max_rx_length> _rx_buf {};

	rfid::Tag _tag {};
};

}	// namespace leka
