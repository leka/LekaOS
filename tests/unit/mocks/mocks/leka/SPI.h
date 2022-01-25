// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPI_MOCK_H_
#define _LEKA_OS_SPI_MOCK_H_

#include <span>

#include "SPI.h"
#include "gmock/gmock.h"

namespace leka {

class SPIMock : public interface::SPI
{
  public:
	MOCK_METHOD(size_t, write, (std::span<uint8_t>), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_SPI_MOCK_H_
