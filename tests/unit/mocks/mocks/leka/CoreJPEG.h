// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/JPEG.hpp"

namespace leka::mock {

class CoreJPEG : public interface::JPEGBase
{
  public:
	MOCK_METHOD(void, initialize, (), (override));

	MOCK_METHOD(JPEG_HandleTypeDef, getHandle, (), (override));
	MOCK_METHOD(JPEG_HandleTypeDef *, getHandlePointer, (), (override));

	MOCK_METHOD(void, decodeImage, (interface::File * file), (override));

	MOCK_METHOD(JPEGImageProperties, getImageProperties, (), (override));
};

}	// namespace leka::mock
