// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_JPEG_H_
#define _LEKA_OS_DRIVER_JPEG_H_

#include <cstdint>
#include <memory>
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
	LKCoreJPEG(LKCoreSTM32HalBase &hal, std::unique_ptr<Mode> mode);

	void initialize(void) final;

	auto getConfig(void) -> JPEG_ConfTypeDef& final;
	auto getHandle(void) -> JPEG_HandleTypeDef& final;

	auto getWidthOffset(void) -> uint32_t final;

	auto decodeImage(LKCoreFatFsBase& file) -> uint32_t final;

	void registerCallbacks(void);

	static auto findFrameOffset(LKCoreFatFsBase& file, uint32_t offset) -> uint32_t;

	struct DMAMode : LKCoreJPEGBase::Mode
	{
		auto decodeImage(JPEG_HandleTypeDef* hjpeg, FIL* file) -> uint32_t final;
		void onMspInitCallback(JPEG_HandleTypeDef* hjpeg) final;
		void onGetDataCallback(JPEG_HandleTypeDef* hjpeg, uint32_t decoded_datasize) final;
		void onDataReadyCallback(JPEG_HandleTypeDef* hjpeg, uint8_t* output_data, uint32_t output_datasize) final;

	private:
		void decoderInputHandler(JPEG_HandleTypeDef* hjpeg, FIL* file);
		bool decoderOutputHandler(JPEG_HandleTypeDef* hjpeg);
		
		DMA_HandleTypeDef _hdma_in;
		DMA_HandleTypeDef _hdma_out;

		struct Buffer
		{
			enum State {Empty, Full};
			State state;  
			uint8_t *data;
			unsigned datasize;
		};

		std::array<Buffer, jpeg::in_buffers_nb> _in_buffers;
		std::array<Buffer, jpeg::out_buffers_nb> _out_buffers;

		uint32_t _out_read_index = 0;
		uint32_t _out_write_index = 0;
		bool _out_paused = 0;

		uint32_t _in_read_index = 0;
		uint32_t _in_write_index = 0;
		bool _in_paused = 0;
	};

	struct PollingMode : LKCoreJPEGBase::Mode
	{
		auto decodeImage(JPEG_HandleTypeDef* hjpeg, FIL* file) -> uint32_t final;
		void onGetDataCallback(JPEG_HandleTypeDef* hjpeg, uint32_t decoded_datasize) final;
		void onDataReadyCallback(JPEG_HandleTypeDef* hjpeg, uint8_t* output_data, uint32_t output_datasize) final;

	private:
		FIL* _file;
		uint32_t _input_file_offset = 0;
		std::array<uint8_t, jpeg::output_data_buffer_size> _jpeg_output_buffer = {0};
		std::array<uint8_t, jpeg::input_data_buffer_size> _jpeg_input_buffer = {0};
	};

private:
	JPEG_HandleTypeDef _hjpeg;
	JPEG_ConfTypeDef _config;

	LKCoreSTM32HalBase& _hal;

	std::unique_ptr<Mode> _mode;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_JPEG_H_
