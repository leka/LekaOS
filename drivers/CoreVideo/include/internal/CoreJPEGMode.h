#ifndef _LEKA_OS_DRIVER_JPEG_MODE_H_
#define _LEKA_OS_DRIVER_JPEG_MODE_H_

#include <array>
#include <cstdint>

#include "LKCoreFatFsBase.h"
#include "LKCoreSTM32HalBase.h"
#include "corevideo_config.h"
#include "st_jpeg_utils.h"
#include "stm32f7xx_hal_jpeg.h"

namespace leka {

struct CoreJPEGMode {
	~CoreJPEGMode() = default;

	virtual auto decodeImage(JPEG_HandleTypeDef *hjpeg, FIL *file) -> uint32_t = 0;

	// called when HAL_JPEG_Init is called
	virtual void onMspInitCallback(JPEG_HandleTypeDef *hjpeg);
	// called when file opened sucessfully to get file info
	virtual void onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info);
	// called when JPEG decoder finished its work
	virtual void onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg);
	// called when error occurs while decoding
	virtual void onErrorCallback(JPEG_HandleTypeDef *hjpeg);
	// called when JPEG decoder is ready to get a new input buffer and process it
	virtual void onGetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t decoded_datasize) = 0;
	// called when JPEG decoder finished processing the current buffer, output data in output_buffer
	virtual void onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_data, uint32_t output_datasize) = 0;

	// color conversion function pointer, set by onInfoReadyCallback
	JPEG_YCbCrToRGB_Convert_Function pConvert_Function;
	uint32_t previous_image_size = 0;

  protected:
	CoreJPEGMode() = default;

	uint32_t _mcu_number	  = 0;
	uint32_t _mcu_block_index = 0;
	bool _hw_decode_ended	  = false;
};

struct CoreJPEGModeDMA : CoreJPEGMode {
	CoreJPEGModeDMA() = default;

	auto decodeImage(JPEG_HandleTypeDef *hjpeg, FIL *file) -> uint32_t final;

	void onMspInitCallback(JPEG_HandleTypeDef *hjpeg) final;
	void onGetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t decoded_datasize) final;
	void onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_data, uint32_t output_datasize) final;

  private:
	void decoderInputHandler(JPEG_HandleTypeDef *hjpeg, FIL *file);
	auto decoderOutputHandler(JPEG_HandleTypeDef *hjpeg) -> bool;

	DMA_HandleTypeDef _hdma_in;
	DMA_HandleTypeDef _hdma_out;

	struct Buffer {
		enum State
		{
			Empty,
			Full
		};
		State state;
		uint8_t *data;
		unsigned datasize;
	};

	std::array<Buffer, jpeg::input_buffers_nb> _in_buffers;
	std::array<Buffer, jpeg::output_buffers_nb> _out_buffers;

	uint32_t _in_buffers_read_index	  = 0;
	uint32_t _in_buffers_write_index  = 0;
	uint32_t _out_buffers_read_index  = 0;
	uint32_t _out_buffers_write_index = 0;

	bool _in_paused	 = false;
	bool _out_paused = false;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_JPEGMODE_H_