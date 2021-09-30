// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVERS_QSPI_H_
#define _LEKA_OS_DRIVERS_QSPI_H_

#include "gmock/gmock.h"
#include "interface/drivers/QSPI.h"

namespace leka::mock {

class QSPI : public interface::QSPI
{
  public:
	MOCK_METHOD(void, setDataTransmissionFormat, (DataTransmissionFormat &), (override));
	MOCK_METHOD(void, setFrequency, (int), (override));

	MOCK_METHOD(size_t, read, (uint8_t, int, uint32_t, lstd::span<uint8_t>), (override));
	MOCK_METHOD(size_t, write, (uint8_t, int, uint32_t, const lstd::span<uint8_t>), (override));

	MOCK_METHOD((std::tuple<size_t, size_t>), sendCommand,
				(uint8_t, uint32_t, const lstd::span<uint8_t>, lstd::span<uint8_t>), (override));
};

}	// namespace leka::mock

#endif	 // _LEKA_OS_DRIVERS_QSPI_H_
