// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_JPEG_DECODE_H_
#define _LEKA_OS_LIB_JPEG_DECODE_H_

#include <cstdint>

#include "LKCoreDMA2DBase.h"
#include "LKCoreFatFsBase.h"
#include "LKCoreSTM32HalBase.h"
#include "st_jpeg_utils.h"
#include "stm32f7xx_hal_jpeg.h"

namespace leka {

class LKCoreJPEG
{
  public:
	LKCoreJPEG(LKCoreSTM32HalBase &hal, LKCoreDMA2DBase &dma2d, LKCoreFatFsBase &file);

	void initialize(void);

	JPEG_ConfTypeDef getConfig(void);
	JPEG_HandleTypeDef getHandle(void);
	JPEG_HandleTypeDef *getHandlePointer(void);

	uint32_t getWidthOffset(void);

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
	LKCoreSTM32HalBase &_hal;
	LKCoreDMA2DBase &_dma2d;
	LKCoreFatFsBase &_file;

	JPEG_YCbCrToRGB_Convert_Function pConvert_Function;

	uint32_t _mcu_number		= 0;
	uint32_t _mcu_block_index	= 0;
	uint32_t _input_file_offset = 0;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_JPEG_DECODE_H_
