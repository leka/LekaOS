// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_JPEG_DECODE_H_
#define _LEKA_OS_LIB_JPEG_DECODE_H_

#include "storage/filesystem/fat/ChaN/ff.h"

#include "LKCoreDMA2D.h"
#include "stm32f7xx_hal_jpeg.h"

void OnError_Handler();

namespace leka {

class LKCoreJPEG
{
  public:
	LKCoreJPEG(LKCoreDMA2D &dma2d);

	void initialize();

	void decodePolling(uint32_t destination_address);
	uint32_t getWidthOffset();
	void getJpegInformation();
	void display(FIL *jpeg_file);

	static FIL *getFile();

  private:
	JPEG_HandleTypeDef _hjpeg;
	JPEG_ConfTypeDef _config;
	LKCoreDMA2D &_dma2d;
	LKCoreDMA2D::LoadParameters _dma2d_params;

	static FIL *_file;
};

// TODO: Make inline with c++17
FIL *LKCoreJPEG::_file = nullptr;

}	// namespace leka

#endif	 // _LEKA_OS_LIB_JPEG_DECODE_H_
