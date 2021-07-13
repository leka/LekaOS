// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DAC_MOCK_H_
#define _LEKA_OS_DAC_MOCK_H_

#include "gmock/gmock.h"
#include "interface/drivers/Dac.h"

namespace leka {

class CoreDACMock : public interface::Dac
{
  public:
	MOCK_METHOD(void, initialize, (), (override));
	MOCK_METHOD(void, deInitialize, (), (override));

	MOCK_METHOD(void, start, (uint16_t * pData, uint32_t dataLength), (override));
	MOCK_METHOD(void, stop, (), (override));
	MOCK_METHOD(DMA_Flag &, dmaFlag, (), (override));
	MOCK_METHOD(DAC_HandleTypeDef &, getHandle, (), (override));
	MOCK_METHOD(DMA_HandleTypeDef &, getDMAHandle, (), (override));

  protected:
	MOCK_METHOD(void, _registerCallbacks, (), (override));
	MOCK_METHOD(void, _halfCptCallback, (), (override));
	MOCK_METHOD(void, _cptCallback, (), (override));
	MOCK_METHOD(void, _registerMspCallbacks, (), (override));
	MOCK_METHOD(void, _mspInitCallback, (), (override));
	MOCK_METHOD(void, _mspDeInitCallback, (), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_DAC_MOCK_H_
