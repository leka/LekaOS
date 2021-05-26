// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_CR95HF_H_
#define _LEKA_OS_DRIVER_LK_CORE_CR95HF_H_

#include "CoreBufferedSerial.h"
#include "RfidInterface.h"

namespace leka {

class CoreCR95HF : public interface::RFID
{
  public:
	explicit CoreCR95HF(interface::BufferedSerial &serial);

	void send(uint8_t *data, const size_t size) final;
	void receive(uint8_t *data, size_t size) final;

  private:
	interface::BufferedSerial &_serial;

	void formatCommand(const uint8_t *data, uint8_t *command, size_t size);
	const size_t findCorrespondingFlag(size_t size);
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_CR95HF_H_
