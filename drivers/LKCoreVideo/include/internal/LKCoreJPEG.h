// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_JPEG_DECODE_H_
#define _LEKA_OS_LIB_JPEG_DECODE_H_

#include "storage/filesystem/fat/ChaN/ff.h"

#include "LKCoreDMA2D.h"
#include "st_jpeg_utils.h"
#include "stm32f7xx_hal_jpeg.h"

namespace leka {

class LKCoreJPEG
{
  public:
	LKCoreJPEG(LKCoreDMA2D &dma2d);

	void initialize(void);

	JPEG_ConfTypeDef getConfig(void);
	JPEG_HandleTypeDef getHandle(void);
	JPEG_HandleTypeDef *getHandlePointer(void);

	uint32_t getWidthOffset(void);

	FIL *getCurrentImage();

	void displayImage(FIL *file);
	HAL_StatusTypeDef decodeImageWithPolling(void);	  // TODO: Update Return type with something else than HAL status

	void onErrorCallback(JPEG_HandleTypeDef *hjpeg);
	void onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info);

	void onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size);
	void onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size);

	void onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg);

  private:
	struct JPEGDataBuffer {
		uint8_t *data;
		uint32_t size;
	};

	uint8_t _mcu_data_output_buffer[leka::jpeg::mcu::output_data_buffer_size] {0};
	uint8_t _jpeg_data_output_buffer[leka::jpeg::input_data_buffer_size] {0};

	JPEGDataBuffer _jpeg_input_buffer = {_jpeg_data_output_buffer, 0};	 // TODO: do we really need this struct?

	JPEG_HandleTypeDef _hjpeg;
	JPEG_ConfTypeDef _config;
	LKCoreDMA2D &_dma2d;

	JPEG_YCbCrToRGB_Convert_Function pConvert_Function;

	uint32_t _mcu_number		= 0;
	uint32_t _mcu_block_index	= 0;
	uint32_t _input_file_offset = 0;

	FIL *_file;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_JPEG_DECODE_H_
