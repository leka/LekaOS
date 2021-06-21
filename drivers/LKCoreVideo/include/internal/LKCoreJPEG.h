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
	LKCoreJPEG(LKCoreSTM32HalBase &hal, LKCoreDMA2DBase &dma2d, LKCoreFatFsBase &file, std::unique_ptr<Mode> mode);

	void initialize(void) final;

	auto getConfig(void) -> JPEG_ConfTypeDef& final;
	auto getHandle(void) -> JPEG_HandleTypeDef& final;
	auto getHandlePointer(void) -> JPEG_HandleTypeDef* final;

	uint32_t getWidthOffset(void) final;

	void playVideo(); // TODO : move this method to LKCoreVideo

	uint32_t decodeImage(void) final;   // TODO: Update Return type with something else than HAL status

	void registerCallbacks(void);

	struct DMAMode : LKCoreJPEGBase::Mode
	{
		auto decodeImage(JPEG_HandleTypeDef *hjpeg, FIL *file) -> uint32_t final; // TODO: Update Return type with something else than HAL status
		void onGetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size) final;
		void onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size) final;

	private:
		void decoderInputHandler(JPEG_HandleTypeDef *hjpeg, FIL *file);
		bool decoderOutputHandler(JPEG_HandleTypeDef *hjpeg);

		enum BufferState {Empty, Full};

		template <unsigned S>
		struct JPEGBuffer {
			unsigned size = 0;
			BufferState state = Empty;
			std::array<uint8_t, S> array = {0};
		};

		uint32_t _out_read_index = 0;
		uint32_t _out_write_index = 0;
		volatile bool _out_paused = false;

		uint32_t _in_read_index = 0;
		uint32_t _in_write_index = 0;
		volatile bool _in_paused = false;

		std::array<JPEGBuffer<jpeg::dma::chunk_size_in>, 2> _jpeg_in_buffers;
		std::array<JPEGBuffer<jpeg::dma::chunk_size_out>, 2> _jpeg_out_buffers;
	};

	struct PollingMode : LKCoreJPEGBase::Mode
	{
		auto decodeImage(JPEG_HandleTypeDef *hjpeg, FIL *file) -> uint32_t final; // TODO: Update Return type with something else than HAL status
		void onGetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size) final;
		void onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size) final;

	private:
		FIL* _file;
		uint32_t _input_file_offset = 0;
		std::array<uint8_t, jpeg::output_data_buffer_size> _jpeg_output_buffer = {0};
		std::array<uint8_t, jpeg::input_data_buffer_size> _jpeg_input_buffer = {0};
	};

private:
	JPEG_HandleTypeDef _hjpeg;
	JPEG_ConfTypeDef _config;

	LKCoreSTM32HalBase &_hal;
	LKCoreDMA2DBase &_dma2d;
	LKCoreFatFsBase &_file;
	std::unique_ptr<Mode> _mode;
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_JPEG_H_
