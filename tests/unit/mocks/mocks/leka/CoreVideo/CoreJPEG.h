// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_JPEG_MOCK_H_
#define _LEKA_OS_JPEG_MOCK_H_

#include "gmock/gmock.h"
#include "interface/CoreJPEG.h"

namespace leka {

class CoreJPEGMock : public interface::CoreJPEG
{
  public:
	MOCK_METHOD(void, initialize, (), (override));

	MOCK_METHOD(void, registerCallbacks, (), (override));

	MOCK_METHOD(uint32_t, decodeImage, (), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_JPEG_MOCK_H_
