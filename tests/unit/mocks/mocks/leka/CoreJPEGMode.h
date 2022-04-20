// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/JPEGMode.hpp"

namespace leka::mock {

class CoreJPEGMode : public interface::JPEGMode
{
  public:
	MOCK_METHOD(void, onMspInitCallback, (JPEG_HandleTypeDef *), (override));
	MOCK_METHOD(void, onInfoReadyCallback, (JPEG_HandleTypeDef *, JPEG_ConfTypeDef *), (override));
	MOCK_METHOD(void, onErrorCallback, (JPEG_HandleTypeDef *), (override));
	MOCK_METHOD(void, onDataReadyCallback, (JPEG_HandleTypeDef *, uint8_t *, uint32_t), (override));
	MOCK_METHOD(void, onDataAvailableCallback, (JPEG_HandleTypeDef *, uint32_t), (override));
	MOCK_METHOD(void, onDecodeCompleteCallback, (JPEG_HandleTypeDef *), (override));

	MOCK_METHOD(size_t, decode, (JPEG_HandleTypeDef *, interface::File &), (override));
};

}	// namespace leka::mock
