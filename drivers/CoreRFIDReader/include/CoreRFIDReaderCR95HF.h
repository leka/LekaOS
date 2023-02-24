// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? CR95HF RFID reader datasheet available at:
// ? https://www.st.com/resource/en/datasheet/cr95hf.pdf

#pragma once

#include "CoreEventQueue.h"
#include "interface/drivers/BufferedSerial.h"
#include "interface/drivers/RFIDReader.h"

namespace leka {

namespace rfid::cr95hf {

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
				settings::idle_tag_detection::tag_detection_command,
				settings::idle_tag_detection::length,
				settings::idle_tag_detection::wu_source,
				settings::idle_tag_detection::enter_control[0],
				settings::idle_tag_detection::enter_control[1],
				settings::idle_tag_detection::wu_control[0],
				settings::idle_tag_detection::wu_control[1],
				settings::idle_tag_detection::leave_control[0],
				settings::idle_tag_detection::leave_control[1],
				settings::idle_tag_detection::wu_periode,
				settings::idle_tag_detection::oscillator_start,
				settings::idle_tag_detection::digital_to_analog_start,
				settings::idle_tag_detection::digital_to_analog_data[0],
				settings::idle_tag_detection::digital_to_analog_data[1],
				settings::idle_tag_detection::swing_count,
				settings::idle_tag_detection::max_sleep,
			});

			inline constexpr auto idn = std::to_array<uint8_t>({
				command::idn::id,
				command::idn::length,
			});

			inline constexpr auto set_baudrate = std::to_array<uint8_t>({
				command::set_baudrate::id,
				command::set_baudrate::length,
			});

			inline constexpr auto set_protocol_iso14443 = std::to_array<uint8_t>({
				command::set_protocol::id,
				command::set_protocol::length,
				protocol::iso14443A.id,
				settings::default_rx_tx_speed,
			});

			inline constexpr auto set_gain_and_modulation = std::to_array<uint8_t>({
				command::set_gain_and_modulation::id,
				command::set_gain_and_modulation::length,
				settings::arc_b,
				settings::flag_increment,
				settings::acr_b_index_for_gain_and_modulation,
				protocol::iso14443A.gain_modulation_values(),
			});

		}	// namespace frame

	}	// namespace command

}	// namespace rfid::cr95hf

class CoreRFIDReaderCR95HF : public interface::RFIDReader
{
  public:
	explicit CoreRFIDReaderCR95HF(interface::BufferedSerial &serial) : _serial(serial) {};

	void init() final;

	void registerOnTagDetectedCallback(const std::function<void()> &callback) final;
	void registerOnTagReadableCallback(const std::function<void(rfid::Tag)> &callback) final;

	void setModeTagDetection() final;
	void setCommunicationProtocol(rfid::Protocol protocol) final;
	void sendRequestToTag(std::span<const uint8_t> data) final;
	auto didTagCommunicationSucceed(size_t sizeTagData) -> bool final;
	auto getTag() -> rfid::Tag & final;
	void onTagReadable() final;

  private:
	void _receiveResponseFromTag();

	void _setProtocolISO14443A();
	void _setGainAndModulationISO14443A();

	rfid::Tag _tag {};
	leka::CoreEventQueue _event_queue {};
	interface::BufferedSerial &_serial;
	std::function<void()> _on_tag_response_available;
	std::function<void(rfid::Tag)> _on_tag_readable;

	std::array<uint8_t, rfid::cr95hf::max_tx_length> _tx_buf {};
	std::array<uint8_t, rfid::cr95hf::max_rx_length> _rx_buf {};
};

}	// namespace leka
