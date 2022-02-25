// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "gmock/gmock.h"
#include "interface/drivers/QSPI.h"

namespace leka::mock {

class QSPI : public interface::QSPI
{
  public:
	MOCK_METHOD(void, setDataTransmissionFormat, (), (override));
	MOCK_METHOD(void, setFrequency, (int), (override));

	MOCK_METHOD(size_t, read, (uint8_t, uint32_t, std::span<uint8_t>, size_t), (override));
	MOCK_METHOD(size_t, write, (uint8_t, uint32_t, const std::span<uint8_t>, size_t), (override));

	MOCK_METHOD((std::tuple<size_t, size_t>), sendCommand,
				(uint8_t, uint32_t, const std::span<uint8_t>, size_t, std::span<uint8_t>, size_t), (override));
};

}	// namespace leka::mock
