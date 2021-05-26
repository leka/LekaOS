// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_CR95HF_H_
#define _LEKA_OS_DRIVER_LK_CORE_CR95HF_H_

#include <cstddef>

#include "CoreBufferedSerial.h"
#include "RfidInterface.h"

namespace leka {

namespace CR95HF {
	constexpr size_t max_data_send_length	 = 16;
	constexpr size_t max_data_receive_length = 32;

	namespace commands {
		constexpr uint8_t set_protocol			  = 0x02;
		constexpr uint8_t send_reveive			  = 0x04;
		constexpr uint8_t set_gain_and_modulation = 0x09;
	}	// namespace commands
	namespace flags {
		constexpr uint8_t seven_significant_bits		  = 0x07;
		constexpr uint8_t eight_significant_bits		  = 0x08;
		constexpr uint8_t eight_significant_bits_with_crc = 0x28;
	}	// namespace flags
}	// namespace CR95HF

class CoreCR95HF : public interface::RFID
{
  public:
	explicit CoreCR95HF(interface::BufferedSerial &serial);

	void send(uint8_t *data, const size_t size) final;
	void receive(uint8_t *data, size_t size) final;

	void setProcoleISO14443();
	void setGainAndModulation();

  private:
	interface::BufferedSerial &_serial;

	std::array<uint8_t, CR95HF::max_data_send_length> _send_buffer {0};
	std::array<uint8_t, CR95HF::max_data_receive_length> _receive_buffer {0};

	void formatCommand(const uint8_t *data, uint8_t *command, size_t size) const;
	[[nodiscard]] auto findCorrespondingFlag(size_t size) const -> uint8_t;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_CR95HF_H_
