// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_JPEG_BASE_H_
#define _LEKA_OS_DRIVER_JPEG_BASE_H_

#include "storage/filesystem/fat/ChaN/ff.h"

#include "LKCoreSTM32HalBase.h"
// #include "stm32f7xx_hal_jpeg.h"

namespace leka {

class LKCoreJPEGBase
{
  public:
	~LKCoreJPEGBase() = default;

	virtual void initialize(void) = 0;

	virtual JPEG_ConfTypeDef getConfig(void)		   = 0;
	virtual JPEG_HandleTypeDef getHandle(void)		   = 0;
	virtual JPEG_HandleTypeDef *getHandlePointer(void) = 0;

	virtual uint32_t getWidthOffset(void) = 0;

	virtual void displayImage(FIL *file) = 0;
	virtual HAL_StatusTypeDef decodeImageWithPolling(
		void) = 0;	 // TODO: Update Return type with something else than HAL status

	virtual void onErrorCallback(JPEG_HandleTypeDef *hjpeg)								= 0;
	virtual void onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) = 0;

	virtual void onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size)					   = 0;
	virtual void onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size) = 0;

	virtual void onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg) = 0;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_JPEG_BASE_H_
