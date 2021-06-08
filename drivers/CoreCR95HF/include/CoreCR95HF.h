// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_CR95HF_H_
#define _LEKA_OS_DRIVER_CORE_CR95HF_H_

#include <lstd_span>

#include "interface/drivers/BufferedSerial.h"
#include "interface/drivers/RFID.h"

namespace leka {

class CoreCR95HF : public interface::RFID
{
  public:
	explicit CoreCR95HF(interface::BufferedSerial &serial) : _serial(serial) {};

	auto init() -> bool final;

	void send(const lstd::span<uint8_t> &iso_command) final;

	auto receive(const lstd::span<uint8_t> &tag_anwser) -> size_t final;

  private:
	interface::BufferedSerial &_serial;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_CR95HF_H_
