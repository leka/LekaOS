// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_JPEGMODE_MOCK_H_
#define _LEKA_OS_JPEGMODE_MOCK_H_

#include "gmock/gmock.h"
#include "interface/CoreJPEGMode.h"

namespace leka {

class CoreJPEGModeMock : public interface::CoreJPEGMode
{
  public:
	MOCK_METHOD(uint32_t, decodeImage, (JPEG_HandleTypeDef * hjpeg, LKCoreFatFsBase &file), (override));

	MOCK_METHOD(void, onMspInitCallback, (JPEG_HandleTypeDef * hjpeg), (override));

	MOCK_METHOD(void, onInfoReadyCallback, (JPEG_HandleTypeDef * hjpeg, JPEG_ConfTypeDef *info), (override));

	MOCK_METHOD(void, onGetDataCallback, (JPEG_HandleTypeDef * hjpeg, uint32_t decoded_datasize), (override));

	MOCK_METHOD(void, onDataReadyCallback, (JPEG_HandleTypeDef * hjpeg, uint8_t *output_data, uint32_t datasize),
				(override));

	MOCK_METHOD(void, onDecodeCompleteCallback, (JPEG_HandleTypeDef * hjpeg), (override));

	MOCK_METHOD(void, onErrorCallback, (JPEG_HandleTypeDef * hjpeg), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_JPEGMODE_MOCK_H_
