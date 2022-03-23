// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/platform/File.h"
#include "stm32f7xx_hal.h"

namespace leka::interface {

struct JPEGMode {
	~JPEGMode() = default;

	virtual auto decodeImage(JPEG_HandleTypeDef *hjpeg, interface::File &file) -> uint32_t = 0;

	// called when HAL_JPEG_Init is called
	virtual void onMspInitCallback(JPEG_HandleTypeDef *hjpeg) = 0;

	// called when file opened sucessfully to get file info
	virtual void onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) = 0;

	// called when JPEG decoder is ready to get a new input buffer and process it
	virtual void onGetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t decoded_datasize) = 0;

	// called when JPEG decoder finished processing the current buffer
	virtual void onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_data, uint32_t datasize) = 0;

	// called when JPEG decoder finished its work
	virtual void onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg) = 0;

	// called when an error occurs while decoding
	virtual void onErrorCallback(JPEG_HandleTypeDef *hjpeg) = 0;
};

}	// namespace leka::interface
