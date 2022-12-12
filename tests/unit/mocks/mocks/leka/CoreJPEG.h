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
	CoreJPEG();
	virtual ~CoreJPEG();

	MOCK_METHOD(void, initialize, (), (override));

	MOCK_METHOD(JPEG_HandleTypeDef &, getHandle, (), (override));

	MOCK_METHOD(size_t, decodeImage, (interface::File &), (override));
	MOCK_METHOD(size_t, findSOIMarker, (interface::File &, std::size_t), (override));

	MOCK_METHOD(JPEGImageProperties, getImageProperties, (), (override));
};

}	// namespace leka::mock
