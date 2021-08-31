// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_JPEG_H_
#define _LEKA_OS_DRIVER_JPEG_H_

#include <cstdint>

#include "LKCoreFatFsBase.h"
#include "LKCoreSTM32HalBase.h"
#include "external/st_jpeg_utils.h"
#include "interface/CoreDMA2D.h"
#include "interface/CoreJPEG.h"

namespace leka {

class CoreJPEG : public interface::CoreJPEG
{
  public:
	CoreJPEG(LKCoreSTM32HalBase &hal, interface::CoreDMA2D &dma2d, LKCoreFatFsBase &file);

	void initialize(void) final;

	JPEG_ConfTypeDef getConfig(void) final;
	JPEG_HandleTypeDef getHandle(void) final;
	JPEG_HandleTypeDef *getHandlePointer(void) final;

	uint32_t getWidthOffset(void) final;

	void displayImage(FIL *file) final;
	HAL_StatusTypeDef decodeImageWithPolling(
		void) final;   // TODO: Update Return type with something else than HAL status

	void onErrorCallback(JPEG_HandleTypeDef *hjpeg) final;
	void onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) final;

	void onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size) final;
	void onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size) final;

	void onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg) final;

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
	interface::CoreDMA2D &_dma2d;
	LKCoreFatFsBase &_file;

	JPEG_YCbCrToRGB_Convert_Function pConvert_Function;

	uint32_t _mcu_number		= 0;
	uint32_t _mcu_block_index	= 0;
	uint32_t _input_file_offset = 0;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_JPEG_H_
