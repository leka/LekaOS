// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_CR95HF_H_
#define _LEKA_OS_DRIVER_CORE_CR95HF_H_

#include <lstd_span>

#include "CoreBufferedSerial.h"
#include "interface/drivers/RFID.h"

namespace leka {

namespace cr95hf {

	struct Protocol {
		const uint8_t id;
		const std::byte gain;
		const std::byte modulation;
		constexpr auto gain_modulation_values() const -> uint8_t { return std::to_integer<uint8_t>(gain | modulation); }
	};

	constexpr size_t max_tx_length = 16;
	constexpr size_t max_rx_length = 32;

	constexpr uint8_t set_protocol_flag		= 0x00;
	constexpr uint8_t arc_b					= 0x68;	  // Analog Register Configuration
	constexpr uint8_t flag_increment		= 0x01;
	constexpr uint8_t gain_modulation_index = 0x01;

	namespace protocol {

		constexpr Protocol ISO15693 = {
			.id = 0x01, .gain = std::byte(0x00), .modulation = std::byte(0xD0)};   // gain = 34 dB, modulation = 95%
		constexpr Protocol ISO14443A = {
			.id = 0x02, .gain = std::byte(0x01), .modulation = std::byte(0xD0)};   // gain = 32 dB, modulation = 95%
		constexpr Protocol ISO14443B = {
			.id = 0x03, .gain = std::byte(0x00), .modulation = std::byte(0x20)};   // gain = 34 dB, modulation = 17%
		constexpr Protocol ISO18092 = {
			.id = 0x04, .gain = std::byte(0x00), .modulation = std::byte(0x20)};   // gain = 34 dB, modulation = 17%

	};	 // namespace protocol

	namespace status {

		constexpr std::array<uint8_t, 2> setup_complete = {0x00, 0x00};

	}	// namespace status

	namespace command {

		constexpr uint8_t set_protocol			  = 0x02;
		constexpr uint8_t send_receive			  = 0x04;
		constexpr uint8_t set_gain_and_modulation = 0x09;

		namespace frame {

			constexpr std::array<uint8_t, 4> set_protocol_ISO14443_command {
				cr95hf::command::set_protocol, 0x02, cr95hf::protocol::ISO14443A.id, cr95hf::set_protocol_flag};

			constexpr std::array<uint8_t, 6> set_gain_and_modulation_command {
				cr95hf::command::set_gain_and_modulation,
				0x04,
				cr95hf::arc_b,
				cr95hf::flag_increment,
				cr95hf::gain_modulation_index,
				cr95hf::protocol::ISO14443A.gain_modulation_values()};

		}	// namespace frame

	}	// namespace command

}	// namespace cr95hf

class CoreCR95HF : public interface::RFID
{
  public:
	explicit CoreCR95HF(interface::BufferedSerial &serial) : _serial(serial) {};

	void send(const lstd::span<uint8_t> &iso_command);
	auto receive(lstd::span<uint8_t> rfid_answer) -> size_t;

	auto init() -> bool;

  private:
	interface::BufferedSerial &_serial;

	void formatCommand(const lstd::span<uint8_t> &iso_command);
	auto calculateCommandSize(const size_t size) const -> size_t;

	void setProtocolISO14443();
	void setGainAndModulation();

	auto isSetupAnswerCorrect() -> bool;

	std::array<uint8_t, cr95hf::max_tx_length> _tx_buf {};
	std::array<uint8_t, cr95hf::max_rx_length> _rx_buf {};
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_CR95HF_H_
