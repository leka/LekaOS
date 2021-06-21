// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_JPEG_BASE_H_
#define _LEKA_OS_DRIVER_JPEG_BASE_H_

#include "storage/filesystem/fat/ChaN/ff.h"

#include "st_jpeg_utils.h"
#include "stm32f7xx_hal_jpeg.h"

namespace leka {

class LKCoreJPEGBase
{
public:
	~LKCoreJPEGBase() = default;

	virtual void initialize(void) = 0;

	virtual auto getConfig(void) -> JPEG_ConfTypeDef& = 0;
	virtual auto getHandle(void) -> JPEG_HandleTypeDef& = 0;
	virtual auto getHandlePointer(void) -> JPEG_HandleTypeDef* = 0;

	virtual auto getWidthOffset(void) -> uint32_t = 0;

	virtual void displayImage(FIL *file) = 0;
	virtual auto decodeImage(void) -> uint32_t = 0;

	virtual void registerCallbacks() = 0;

protected:
	struct Mode {
		~Mode() = default;

		virtual auto decodeImage(JPEG_HandleTypeDef *hjpeg, FIL* file) -> uint32_t = 0;

		// called when file opened sucessfully to get file info
		virtual void onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info);
		// called when JPEG decoder finished its work
		virtual void onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg);
		// called when error occurs while decoding
		virtual void onErrorCallback(JPEG_HandleTypeDef *hjpeg);
		// called when JPEG decoder is ready to get a new inpu buffer and process it
		virtual void onGetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size) = 0;
		// called when JPEG decoder finished processing the current buffer, output data in output_buffer
		virtual void onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size) = 0;

		// conversion function, defined on runtime depending on file subsampling
		JPEG_YCbCrToRGB_Convert_Function pConvert_Function;

		uint32_t _previous_image_size 	= 0;

	protected:
		uint32_t _mcu_number = 0;
		uint32_t _mcu_block_index = 0;
		bool _hw_decode_ended = false;
	};
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_JPEG_BASE_H_
