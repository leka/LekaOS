// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_CR95HF_H_
#define _LEKA_OS_DRIVER_CORE_CR95HF_H_

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

	}	// namespace status

	namespace command {

		constexpr uint8_t send_receive			  = 0x04;
		constexpr uint8_t set_protocol			  = 0x02;
		constexpr uint8_t set_gain_and_modulation = 0x09;

		namespace frame {

			constexpr std::array<uint8_t, 4> set_protocol_iso14443 {
				rfid::cr95hf::command::set_protocol, 0x02, rfid::cr95hf::protocol::iso14443A.id,
				rfid::cr95hf::settings::default_protocol_parameters_for_rx_speed_tx_speed_rfu};

			constexpr std::array<uint8_t, 6> set_gain_and_modulation {
				rfid::cr95hf::command::set_gain_and_modulation,
				0x04,
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

	auto init() -> bool final;

	void send(const lstd::span<uint8_t> &iso_command) final;

	auto receive(const lstd::span<uint8_t> &anwser) -> size_t final;

  private:
	auto didSetupSucceed() -> bool;

	void setProtocolISO14443();
	void setGainAndModulation();
	auto receiveCR95HFAnswer() -> size_t;

	auto formatCommand(const lstd::span<uint8_t> &command) -> size_t;
	auto processTagAnswer(const lstd::span<uint8_t> &tag_anwser, const size_t size) -> bool;

	interface::BufferedSerial &_serial;

	std::array<uint8_t, rfid::cr95hf::max_tx_length> _tx_buf {};
	std::array<uint8_t, rfid::cr95hf::max_rx_length> _rx_buf {};
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_CR95HF_H_
