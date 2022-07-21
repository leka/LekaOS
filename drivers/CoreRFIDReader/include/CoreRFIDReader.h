// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

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

	inline constexpr auto max_tx_length = std::size_t {16};
	inline constexpr auto max_rx_length = std::size_t {32};

	namespace settings {

		inline constexpr auto default_rx_tx_speed				  = uint8_t {0x00};
		inline constexpr auto arc_b								  = uint8_t {0x68};	  // Analog Register Configuration
		inline constexpr auto flag_increment					  = uint8_t {0x01};
		inline constexpr auto acr_b_index_for_gain_and_modulation = uint8_t {0x01};

		namespace idle_tag_detection {

			inline constexpr auto tag_detection_command	  = uint8_t {0x07};
			inline constexpr auto wu_source				  = uint8_t {0x0A};
			inline constexpr auto length				  = uint8_t {0x0E};
			inline constexpr auto enter_control			  = std::to_array<uint8_t>({0x21, 0x00});
			inline constexpr auto wu_control			  = std::to_array<uint8_t>({0x79, 0x01});
			inline constexpr auto leave_control			  = std::to_array<uint8_t>({0x18, 0x00});
			inline constexpr auto wu_periode			  = uint8_t {0x20};
			inline constexpr auto oscillator_start		  = uint8_t {0x60};
			inline constexpr auto digital_to_analog_start = uint8_t {0x60};
			inline constexpr auto digital_to_analog_data  = std::to_array<uint8_t>({0x70, 0x80});
			inline constexpr auto swing_count			  = uint8_t {0x3F};
			inline constexpr auto max_sleep				  = uint8_t {0x08};

		}	// namespace idle_tag_detection

	}	// namespace settings

	namespace tag_answer {

		inline constexpr auto heading_size = std::size_t {2};
		inline constexpr auto flag_size	   = std::size_t {3};

	}	// namespace tag_answer

	namespace expected_answer_size {

		inline constexpr auto idn						 = std::size_t {17};
		inline constexpr auto tag_detection				 = std::size_t {3};
		inline constexpr auto set_baudrate				 = std::size_t {1};
		inline constexpr auto set_communication_protocol = std::size_t {2};

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

		inline constexpr auto communication_succeed	 = uint8_t {0x80};
		inline constexpr auto error_time_out		 = uint8_t {0x87};
		inline constexpr auto idn_success			 = std::to_array<uint8_t>({0x00, 0x0F});
		inline constexpr auto setup_success			 = std::to_array<uint8_t>({0x00, 0x00});
		inline constexpr auto tag_detection_callback = std::to_array<uint8_t>({0x01, 0x02});

	}	// namespace status

	namespace command {

		inline constexpr auto send_receive = uint8_t {0x04};

		namespace idn {
			inline constexpr auto id	 = uint8_t {0x01};
			inline constexpr auto length = uint8_t {0x00};
		}	// namespace idn

		namespace set_baudrate {
			inline constexpr auto id	 = uint8_t {0x0A};
			inline constexpr auto length = uint8_t {0x01};
		}	// namespace set_baudrate

		namespace set_protocol {
			inline constexpr auto id	 = uint8_t {0x02};
			inline constexpr auto length = uint8_t {0x02};
		}	// namespace set_protocol

		namespace set_gain_and_modulation {
			inline constexpr auto id	 = uint8_t {0x09};
			inline constexpr auto length = uint8_t {0x04};
		}	// namespace set_gain_and_modulation

		namespace frame {
			inline constexpr auto set_tag_detection_mode = std::to_array<uint8_t>({
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
			});

			inline constexpr auto idn = std::to_array<uint8_t>({
				rfid::command::idn::id,
				rfid::command::idn::length,
			});

			inline constexpr auto set_baudrate = std::to_array<uint8_t>({
				rfid::command::set_baudrate::id,
				rfid::command::set_baudrate::length,
			});

			inline constexpr auto set_protocol_iso14443 = std::to_array<uint8_t>({
				rfid::command::set_protocol::id,
				rfid::command::set_protocol::length,
				rfid::protocol::iso14443A.id,
				rfid::settings::default_rx_tx_speed,
			});

			inline constexpr auto set_gain_and_modulation = std::to_array<uint8_t>({
				rfid::command::set_gain_and_modulation::id,
				rfid::command::set_gain_and_modulation::length,
				rfid::settings::arc_b,
				rfid::settings::flag_increment,
				rfid::settings::acr_b_index_for_gain_and_modulation,
				rfid::protocol::iso14443A.gain_modulation_values(),
			});

		}	// namespace frame

	}	// namespace command

}	// namespace rfid

class CoreRFIDReader : public interface::RFIDReader
{
  public:
	explicit CoreRFIDReader(interface::BufferedSerial &serial) : _serial(serial) {};

	void init() final;

	void registerOnTagDetectedCallback(const std::function<void()> &callback) final;
	void registerOnTagReadableCallback(const std::function<void()> &callback) final;

	auto isTagDetected() -> bool final;
	void setTagDetectionMode() final;
	void setCommunicationProtocol(rfid::Protocol protocol) final;
	void sendToTag(std::span<const uint8_t> data) final;
	auto didTagCommunicationSucceed(size_t sizeTagData) -> bool final;
	void getDataFromTag(std::span<uint8_t> data) final;
	auto getTag() -> rfid::Tag final;
	void onTagDataReceived() final;

  private:
	void registerOnDataAvailableCallback();
	void onDataAvailable();

	void read();

	void setProtocolISO14443A();
	void setGainAndModulationISO14443A();

	rfid::Tag _tag {};
	size_t _anwser_size {0};
	rtos::Thread _thread {};
	events::EventQueue _event_queue {};
	interface::BufferedSerial &_serial;
	std::function<void()> _on_tag_detected;
	std::function<void()> _on_tag_valid;

	std::array<uint8_t, rfid::max_tx_length> _tx_buf {};
	std::array<uint8_t, rfid::max_rx_length> _rx_buf {};
};

}	// namespace leka
