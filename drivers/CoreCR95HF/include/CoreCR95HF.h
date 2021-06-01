// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_CR95HF_H_
#define _LEKA_OS_DRIVER_CORE_CR95HF_H_

#include "rtos/ThisThread.h"

#include "CoreBufferedSerial.h"
#include "RFID.h"

namespace leka {

struct Protocol {
	const uint8_t id;
	const std::byte gain;
	const std::byte modulation;
	constexpr auto gain_modulation_values() const -> std::byte { return gain | modulation; }
};

namespace cr95hf {

	constexpr size_t max_tx_length = 16;
	constexpr size_t max_rx_length = 32;

	constexpr uint8_t set_protocol_flag		= 0x00;
	constexpr uint8_t arc_b					= 0x68;	  // Analog Register Configuration
	constexpr uint8_t flag_increment		= 0x01;
	constexpr uint8_t gain_modulation_index = 0x01;

	namespace command {

		constexpr uint8_t set_protocol			  = 0x02;
		constexpr uint8_t send_receive			  = 0x04;
		constexpr uint8_t set_gain_and_modulation = 0x09;

	}	// namespace command

	namespace protocol {

		constexpr Protocol ISO15693	 = {.id			= 0x01,
										.gain		= static_cast<std::byte>(0x00),
										.modulation = static_cast<std::byte>(0xD0)};   // gain = 34 dB, modulation = 95%
		constexpr Protocol ISO14443A = {.id			= 0x02,
										.gain		= static_cast<std::byte>(0x01),
										.modulation = static_cast<std::byte>(0xD0)};   // gain = 32 dB, modulation = 95%
		constexpr Protocol ISO14443B = {.id			= 0x03,
										.gain		= static_cast<std::byte>(0x00),
										.modulation = static_cast<std::byte>(0x20)};   // gain = 34 dB, modulation = 17%
		constexpr Protocol ISO18092	 = {.id			= 0x04,
										.gain		= static_cast<std::byte>(0x00),
										.modulation = static_cast<std::byte>(0x20)};   // gain = 34 dB, modulation = 17%

	};	 // namespace protocol

}	// namespace cr95hf

class CoreCR95HF : public interface::RFID
{
  public:
	explicit CoreCR95HF(interface::BufferedSerial &serial) : _serial(serial) {};

	template <size_t SIZE>
	void send(const std::array<uint8_t, SIZE> &cmd_iso)
	{
		formatCommand(cmd_iso);
		int number_of_bytes_added_to_the_command = 2;

		if (const size_t command_size = cmd_iso.size() + number_of_bytes_added_to_the_command;
			command_size <= cr95hf::max_tx_length) {
			_serial.write(_tx_buf.data(), command_size);
		} else {
			_serial.write(_tx_buf.data(), 1);
		}
	}

	template <size_t SIZE>
	void receive(std::array<uint8_t, SIZE> &rfid_answer)
	{
		auto rfid_answer_size = rfid_answer.size();
		_serial.read(_rx_buf.data(), rfid_answer_size);

		for (auto i = 0; i < rfid_answer_size; ++i) {
			rfid_answer[i] = _rx_buf[i];
		}
	}

	auto init() -> bool;

  private:
	interface::BufferedSerial &_serial;

	std::array<uint8_t, cr95hf::max_tx_length> _tx_buf {};
	std::array<uint8_t, cr95hf::max_rx_length> _rx_buf {};

	template <size_t SIZE>
	void formatCommand(std::array<uint8_t, SIZE> cmd_iso)
	{
		_tx_buf[0] = cr95hf::command::send_receive;
		_tx_buf[1] = cmd_iso.size();

		for (auto i = 0; i < cmd_iso.size(); ++i) {
			_tx_buf[i + 2] = cmd_iso[i];
		}
	}

	void setProtocoleISO14443();
	void setGainAndModulation();

	auto checkSensorSetup(const uint8_t *buffer) const -> bool;
	auto receiveSetupAnswer() -> bool;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_CR95HF_H_
