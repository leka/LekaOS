// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_CR95HF_H_
#define _LEKA_OS_DRIVER_CORE_CR95HF_H_

#include "rtos/ThisThread.h"

#include "CoreBufferedSerial.h"
#include "RFID.h"

namespace leka {

namespace cr95hf {

	struct Protocol {
		const uint8_t id;
		const std::byte gain;
		const std::byte modulation;
		constexpr auto gain_modulation_values() const -> std::uint8_t
		{
			return std::to_integer<uint8_t>(gain | modulation);
		}
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

	namespace command {

		constexpr uint8_t set_protocol			  = 0x02;
		constexpr uint8_t send_receive			  = 0x04;
		constexpr uint8_t set_gain_and_modulation = 0x09;

		constexpr std::array<uint8_t, 4> set_protocol_ISO14443_command {
			cr95hf::command::set_protocol, 0x02, cr95hf::protocol::ISO14443A.id, cr95hf::set_protocol_flag};

		constexpr std::array<uint8_t, 6> set_gain_and_modulation_command {
			cr95hf::command::set_gain_and_modulation,
			0x04,
			cr95hf::arc_b,
			cr95hf::flag_increment,
			cr95hf::gain_modulation_index,
			cr95hf::protocol::ISO14443A.gain_modulation_values()};

	}	// namespace command

}	// namespace cr95hf

class CoreCR95HF : public interface::RFID
{
  public:
	explicit CoreCR95HF(interface::BufferedSerial &serial) : _serial(serial) {};

	template <size_t SIZE>
	void send(const std::array<uint8_t, SIZE> &iso_command)
	{
		auto size = calculateCommandSize(iso_command.size());
		formatCommand(iso_command);

		_serial.write(_tx_buf.data(), size);
	}

	template <size_t SIZE>
	void receive(std::array<uint8_t, SIZE> &rfid_answer)
	{
		_serial.read(_rx_buf.data(), rfid_answer.size());

		for (auto i = 0; i < rfid_answer.size(); ++i) {
			rfid_answer[i] = _rx_buf[i];
		}
	}

	auto init() -> bool;

  private:
	interface::BufferedSerial &_serial;

	void setProtocoleISO14443();
	void setGainAndModulation();

	auto isSetupAnswerCorrect() -> bool;
	template <size_t SIZE>
	auto checkSensorSetup(const std::array<uint8_t, SIZE> &data) const -> bool
	{
		const std::array<uint8_t, 2> CR95HF_setup_completed = {0x00, 0x00};

		if (data[0] == CR95HF_setup_completed[0] && data[1] == CR95HF_setup_completed[1]) {
			return true;
		}

		return false;
	};

	auto calculateCommandSize(const size_t iso_cmd_size) const -> size_t;

	template <size_t SIZE>
	void formatCommand(std::array<uint8_t, SIZE> cmd_iso)
	{
		_tx_buf[0] = cr95hf::command::send_receive;
		_tx_buf[1] = cmd_iso.size();

		for (auto i = 0; i < cmd_iso.size(); ++i) {
			_tx_buf[i + 2] = cmd_iso[i];
		}
	}

	std::array<uint8_t, cr95hf::max_tx_length> _tx_buf {};
	std::array<uint8_t, cr95hf::max_rx_length> _rx_buf {};
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_CR95HF_H_
