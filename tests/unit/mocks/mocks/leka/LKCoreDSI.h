// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DSI_MOCK_H_
#define _LEKA_OS_DSI_MOCK_H_

#include "interface/LKCoreDSI.h"
#include "gmock/gmock.h"

namespace leka {

class LKCoreDSIMock : public interface::LKCoreDSI
{
  public:
	MOCK_METHOD(void, initialize, (), (override));
	MOCK_METHOD(void, start, (), (override));
	MOCK_METHOD(void, reset, (), (override));
	MOCK_METHOD(DSI_VidCfgTypeDef, getConfig, (), (override));
	MOCK_METHOD(void, write, (const uint8_t *data, const uint32_t size), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_DSI_MOCK_H_
