// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_JPEG_DECODE_H_
#define _LEKA_OS_LIB_JPEG_DECODE_H_

#include "storage/filesystem/fat/ChaN/ff.h"

#include "LKCoreDMA2D.h"
#include "st_jpeg_utils.h"
#include "stm32f7xx_hal_jpeg.h"

void OnError_Handler();

namespace leka {

class LKCoreJPEG
{
  public:
	LKCoreJPEG(LKCoreDMA2D &dma2d);

	void initialize();

	HAL_StatusTypeDef decodeImageWithPolling(void);
	uint32_t getWidthOffset(void);

	void display(FIL *jpeg_file);

	// TODO: remove static if possible
	static FIL *getFile();

	void onErrorCallback(JPEG_HandleTypeDef *hjpeg);
	void onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info);

	void onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size);
	void onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size);

	void onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg);

  private:
	typedef struct {
		uint8_t *DataBuffer;
		uint32_t DataBufferSize;

	} JPEG_Data_BufferTypeDef;

	static const uint32_t CHUNK_SIZE_IN	 = 4096;
	static const uint32_t CHUNK_SIZE_OUT = 768;

	uint8_t MCU_Data_OutBuffer[CHUNK_SIZE_OUT];
	uint8_t JPEG_Data_InBuffer[CHUNK_SIZE_IN];

	// TODO: do we really need this struct?
	JPEG_Data_BufferTypeDef JPEG_InBuffer = {JPEG_Data_InBuffer, 0};

	JPEG_HandleTypeDef _hjpeg;
	JPEG_ConfTypeDef _config;
	LKCoreDMA2D &_dma2d;

	JPEG_YCbCrToRGB_Convert_Function pConvert_Function;

	// TODO: rename variables
	uint32_t MCU_TotalNb	   = 0;
	uint32_t MCU_BlockIndex	   = 0;
	uint32_t Inputfile_Offset  = 0;
	uint32_t Jpeg_Decoding_End = 0;
	uint32_t FrameBufferAddress;

	static FIL *_file;
};

// TODO: Make inline with c++17
FIL *LKCoreJPEG::_file = nullptr;

}	// namespace leka

#endif	 // _LEKA_OS_LIB_JPEG_DECODE_H_
