// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_JPEG_H_
#define _LEKA_OS_DRIVER_JPEG_H_

#include <cstdint>
#include <array>

#include "LKCoreDMA2DBase.h"
#include "LKCoreFatFsBase.h"
#include "LKCoreJPEGBase.h"
#include "LKCoreSTM32HalBase.h"
#include "st_jpeg_utils.h"

namespace leka {

class LKCoreJPEG : public LKCoreJPEGBase
{
	public:
	LKCoreJPEG(LKCoreSTM32HalBase &hal, LKCoreDMA2DBase &dma2d, LKCoreFatFsBase &file);

	void initialize(void) final;

	JPEG_ConfTypeDef getConfig(void) final;
	JPEG_HandleTypeDef getHandle(void) final;
	JPEG_HandleTypeDef *getHandlePointer(void) final;

	uint32_t getWidthOffset(void) final;

	void displayImage(FIL *file) final;
	void playVideo();

	HAL_StatusTypeDef decodeImageWithPolling(void) final;   // TODO: Update Return type with something else than HAL status
	HAL_StatusTypeDef decodeImageWithDma(void);

	void registerPollingCallbacks(void);
	void registerDmaCallbacks(void);

	void polling_onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info);
	void polling_onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size);
	void polling_onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size);
	void polling_onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg);
	void polling_onErrorCallback(JPEG_HandleTypeDef *hjpeg);

	void dma_onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info);
	void dma_onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size);
	void dma_onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size);
	void dma_onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg);
	void dma_onErrorCallback(JPEG_HandleTypeDef *hjpeg);

	void onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) final {}
	void onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size) final {}
	void onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size) final {}
	void onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg) final {}
	void onErrorCallback(JPEG_HandleTypeDef *hjpeg) final {}

	private:
	struct JPEGDataBuffer {
		uint8_t *data;
		uint32_t size;
	};

	std::array<uint8_t, jpeg::output_data_buffer_size> _jpeg_output_buffer = {0};
	std::array<uint8_t, jpeg::input_data_buffer_size> _jpeg_input_buffer = {0};

	JPEG_HandleTypeDef _hjpeg;
	JPEG_ConfTypeDef _config;

	LKCoreSTM32HalBase &_hal;
	LKCoreDMA2DBase &_dma2d;
	LKCoreFatFsBase &_file;

	JPEG_YCbCrToRGB_Convert_Function pConvert_Function;

	uint32_t _previous_frame_size 	= 0;
	uint32_t _mcu_number			= 0;
	uint32_t _mcu_block_index		= 0;
	uint32_t _input_file_offset 	= 0;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_JPEG_H_
