// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_CR95HF_H_
#define _LEKA_OS_DRIVER_CORE_CR95HF_H_

#include <cstdint>
#include <lstd_span>

#include "interface/drivers/BufferedSerial.h"
#include "interface/drivers/RFID.h"

namespace leka {

namespace rfid::cr95hf {

	struct Protocol {
		const uint8_t id;
		const std::byte gain;
		const std::byte modulation;
		constexpr auto gain_modulation_values() const -> uint8_t { return std::to_integer<uint8_t>(gain | modulation); }
	};

	constexpr size_t max_tx_length = 16;
	constexpr size_t max_rx_length = 32;

	namespace settings {

		constexpr uint8_t default_protocol_parameters_for_rx_speed_tx_speed_rfu = 0x00;
		constexpr uint8_t arc_b								  = 0x68;	// Analog Register Configuration
		constexpr uint8_t flag_increment					  = 0x01;
		constexpr uint8_t acr_b_index_for_gain_and_modulation = 0x01;

		namespace idle_tag_detection {

			constexpr uint8_t tag_detection_command					= 0x07;
			constexpr uint8_t wu_source								= 0x0A;
			constexpr uint8_t length								= 0x0E;
			constexpr std::array<uint8_t, 2> enter_control			= {0x21, 0x00};
			constexpr std::array<uint8_t, 2> wu_control				= {0x79, 0x01};
			constexpr std::array<uint8_t, 2> leave_control			= {0x18, 0x00};
			constexpr uint8_t wu_periode							= 0x20;
			constexpr uint8_t oscillator_start						= 0x60;
			constexpr uint8_t digital_to_analog_start				= 0x60;
			constexpr std::array<uint8_t, 2> digital_to_analog_data = {0x70, 0x80};
			constexpr uint8_t swing_count							= 0x3F;
			constexpr uint8_t max_sleep								= 0x08;

		}	// namespace idle_tag_detection

	}	// namespace settings

	namespace tag_answer {

		constexpr size_t heading_size = 2;
		constexpr size_t flag_size	  = 3;

	}	// namespace tag_answer

	namespace protocol {

		constexpr Protocol iso15693 = {
			.id = 0x01, .gain = std::byte {0x00}, .modulation = std::byte {0xD0}};	 // gain = 34 dB, modulation = 95%
		constexpr Protocol iso14443A = {
			.id = 0x02, .gain = std::byte {0x01}, .modulation = std::byte {0xD0}};	 // gain = 32 dB, modulation = 95%
		constexpr Protocol iso14443B = {
			.id = 0x03, .gain = std::byte {0x00}, .modulation = std::byte {0x20}};	 // gain = 34 dB, modulation = 17%
		constexpr Protocol iso18092 = {
			.id = 0x04, .gain = std::byte {0x00}, .modulation = std::byte {0x20}};	 // gain = 34 dB, modulation = 17%

	};	 // namespace protocol

	namespace status {

		constexpr uint8_t communication_succeed		   = 0x80;
		constexpr uint8_t error_time_out			   = 0x87;
		constexpr std::array<uint8_t, 2> setup_success = {0x00, 0x00};
		constexpr std::array<uint8_t, 2> tag_detection_callback {0x01, 0x02};

	}	// namespace status

	namespace command {

		constexpr uint8_t send_receive = 0x04;

		namespace set_protocol {
			constexpr uint8_t id	 = 0x02;
			constexpr uint8_t length = 0x02;
		}	// namespace set_protocol

		namespace set_gain_and_modulation {
			constexpr uint8_t id	 = 0x09;
			constexpr uint8_t length = 0x04;
		}	// namespace set_gain_and_modulation

		namespace frame {
			constexpr std::array<uint8_t, 16> set_mode_tag_detection {
				rfid::cr95hf::settings::idle_tag_detection::tag_detection_command,
				rfid::cr95hf::settings::idle_tag_detection::length,
				rfid::cr95hf::settings::idle_tag_detection::wu_source,
				rfid::cr95hf::settings::idle_tag_detection::enter_control[0],
				rfid::cr95hf::settings::idle_tag_detection::enter_control[1],
				rfid::cr95hf::settings::idle_tag_detection::wu_control[0],
				rfid::cr95hf::settings::idle_tag_detection::wu_control[1],
				rfid::cr95hf::settings::idle_tag_detection::leave_control[0],
				rfid::cr95hf::settings::idle_tag_detection::leave_control[1],
				rfid::cr95hf::settings::idle_tag_detection::wu_periode,
				rfid::cr95hf::settings::idle_tag_detection::oscillator_start,
				rfid::cr95hf::settings::idle_tag_detection::digital_to_analog_start,
				rfid::cr95hf::settings::idle_tag_detection::digital_to_analog_data[0],
				rfid::cr95hf::settings::idle_tag_detection::digital_to_analog_data[1],
				rfid::cr95hf::settings::idle_tag_detection::swing_count,
				rfid::cr95hf::settings::idle_tag_detection::max_sleep};

			constexpr std::array<uint8_t, 4> set_protocol_iso14443 {
				rfid::cr95hf::command::set_protocol::id, rfid::cr95hf::command::set_protocol::length,
				rfid::cr95hf::protocol::iso14443A.id,
				rfid::cr95hf::settings::default_protocol_parameters_for_rx_speed_tx_speed_rfu};

			constexpr std::array<uint8_t, 6> set_gain_and_modulation {
				rfid::cr95hf::command::set_gain_and_modulation::id,
				rfid::cr95hf::command::set_gain_and_modulation::length,
				rfid::cr95hf::settings::arc_b,
				rfid::cr95hf::settings::flag_increment,
				rfid::cr95hf::settings::acr_b_index_for_gain_and_modulation,
				rfid::cr95hf::protocol::iso14443A.gain_modulation_values()};

		}	// namespace frame

	}	// namespace command

}	// namespace rfid::cr95hf

class CoreCR95HF : public interface::RFID
{
  public:
	explicit CoreCR95HF(interface::BufferedSerial &serial) : _serial(serial) {};

	void init() final { registerSigioCallback(); }

	void registerTagAvailableCallback(tagAvailableCallback callback) final { _tagAvailableCallback = callback; };
	auto getTagAvailableCallback() -> tagAvailableCallback final { return _tagAvailableCallback; };

	auto setCommunicationProtocol(rfid::Protocol protocol) -> bool final;

	void sendCommandToTag(lstd::span<uint8_t> iso_command) final;
	auto receiveDataFromTag(lstd::span<uint8_t> answer) -> size_t final;

  private:
	void registerSigioCallback();
	void onTagAvailable();
	auto receiveTagDetectionCallback() -> bool;
	void setModeTagDetection();

	auto writeConfiguration(lstd::span<uint8_t> conf) -> size_t;

	auto setProtocolISO14443A() -> bool;
	auto setGainAndModulationISO14443A() -> bool;

	auto didSetupSucceed() -> bool;
	auto receiveCR95HFAnswer() -> size_t;

	auto formatCommand(lstd::span<uint8_t> command) -> size_t;

	auto DataFromTagIsCorrect(size_t sizeTagAnswer) -> bool;
	void copyTagDataToSpan(lstd::span<uint8_t> answer);

	tagAvailableCallback _tagAvailableCallback;

	interface::BufferedSerial &_serial;

	std::array<uint8_t, rfid::cr95hf::max_tx_length> _tx_buf {};
	std::array<uint8_t, rfid::cr95hf::max_rx_length> _rx_buf {};

	rfid::Tag _tag {};
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_CR95HF_H_
