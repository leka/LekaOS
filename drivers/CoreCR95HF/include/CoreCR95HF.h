// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_CR95HF_H_
#define _LEKA_OS_DRIVER_LK_CORE_CR95HF_H_

#include "rtos/ThisThread.h"

#include "CoreBufferedSerial.h"
#include "RFID.h"

namespace leka {

namespace cr95hf {

	constexpr size_t max_tx_length = 16;
	constexpr size_t max_rx_length = 32;

	constexpr uint8_t protocol_flag			= 0x00;
	constexpr uint8_t arc_b					= 0x68;	  // Analog Register Configuration
	constexpr uint8_t flag_increment		= 0x01;
	constexpr uint8_t gain_modulation_index = 0x01;

	namespace command {

		constexpr uint8_t set_protocol			  = 0x02;
		constexpr uint8_t send_receive			  = 0x04;
		constexpr uint8_t set_gain_and_modulation = 0x09;

	}	// namespace command

	namespace protocol {

		constexpr uint8_t ISO15693	= 0x01;
		constexpr uint8_t ISO14443A = 0x02;
		constexpr uint8_t ISO14443B = 0x03;
		constexpr uint8_t ISO18092	= 0x04;

	}	// namespace protocol

	namespace gain_modulation_values {

		constexpr uint8_t ISO15993	= 0xD0;
		constexpr uint8_t ISO14443A = 0xD1;
		constexpr uint8_t ISO14443B = 0x20;
		constexpr uint8_t ISO18092	= 0x20;

	}	// namespace gain_modulation_values

}	// namespace cr95hf

class CoreCR95HF : public interface::RFID
{
  public:
	explicit CoreCR95HF(interface::BufferedSerial &serial) : _serial(serial) {};

	template <size_t SIZE>
	void send(interface::CommandISO<SIZE> cmd)
	{
		if (const size_t command_size = cmd.data.size() + 3; command_size > cr95hf::max_tx_length) {
			_serial.write(formatedCommand(cmd), 1);
		}

		else {
			_serial.write(formatedCommand(cmd), command_size);
		}
	}

	void receive(uint8_t *data, size_t size) final;
	auto init() -> bool;

  private:
	interface::BufferedSerial &_serial;

	std::array<uint8_t, cr95hf::max_tx_length> _tx_buf {};
	std::array<uint8_t, cr95hf::max_rx_length> _rx_buf {};

	template <size_t SIZE>
	auto formatedCommand(interface::CommandISO<SIZE> cmd) -> const uint8_t *
	{
		_tx_buf[0] = cr95hf::command::send_receive;
		_tx_buf[1] = static_cast<uint8_t>(cmd.data.size()) + 1;

		for (auto i = 0; i < cmd.data.size(); ++i) {
			_tx_buf[i + 2] = cmd.getData()[i];
		}

		_tx_buf[cmd.data.size() + 2] = static_cast<uint8_t>(cmd.flags);

		return _tx_buf.data();
	}

	void setProcoleISO14443();
	void setGainAndModulation();

	auto checkSensorSetup(const uint8_t *buffer) const -> bool;
	auto receiveSetupAnswer() -> bool;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_CR95HF_H_
