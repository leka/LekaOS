// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <chrono>

#include "rtos/ThisThread.h"

#include "CoreVideo.h"
#include "LogKit.h"

using namespace std::chrono;
using namespace leka;

CoreVideo::CoreVideo(LKCoreSTM32HalBase &hal, interface::CoreSDRAM &coresdram, interface::CoreDMA2D &coredma2d,
						 interface::CoreDSI &coredsi, interface::CoreLTDC &coreltdc, interface::CoreLCD &corelcd,
						 interface::CoreJPEG &corejpeg)
	: _hal(hal),
	  _coresdram(coresdram),
	  _coredma2d(coredma2d),
	  _coreltdc(coreltdc),
	  _coredsi(coredsi),
	  _corelcd(corelcd),
	  _corejpeg(corejpeg)
{
}

void CoreVideo::initialize()
{
	/** @brief Enable the LTDC clock */
	_hal.HAL_RCC_LTDC_CLK_ENABLE();

	/** @brief Toggle Sw reset of LTDC IP */
	_hal.HAL_RCC_LTDC_FORCE_RESET();
	_hal.HAL_RCC_LTDC_RELEASE_RESET();

	/** @brief Enable the DMA2D clock */
	_hal.HAL_RCC_DMA2D_CLK_ENABLE();

	/** @brief Toggle Sw reset of DMA2D IP */
	_hal.HAL_RCC_DMA2D_FORCE_RESET();
	_hal.HAL_RCC_DMA2D_RELEASE_RESET();

	/** @brief Enable DSI Host and wrapper clocks */
	_hal.HAL_RCC_DSI_CLK_ENABLE();

	/** @brief Soft Reset the DSI Host and wrapper */
	_hal.HAL_RCC_DSI_FORCE_RESET();
	_hal.HAL_RCC_DSI_RELEASE_RESET();

	/** @brief NVIC configuration for LTDC interrupt that is now enabled */
	_hal.HAL_NVIC_SetPriority(LTDC_IRQn, 3, 0);
	_hal.HAL_NVIC_EnableIRQ(LTDC_IRQn);

	/** @brief NVIC configuration for DMA2D interrupt that is now enabled */
	_hal.HAL_NVIC_SetPriority(DMA2D_IRQn, 3, 0);
	_hal.HAL_NVIC_EnableIRQ(DMA2D_IRQn);

	/** @brief NVIC configuration for DSI interrupt that is now enabled */
	_hal.HAL_NVIC_SetPriority(DSI_IRQn, 3, 0);
	_hal.HAL_NVIC_EnableIRQ(DSI_IRQn);

	_coresdram.initialize();

	_corejpeg.initialize();
	_coredma2d.initialize();

	_coreltdc.initialize();

	_coredsi.initialize();

	_corelcd.initialize();
	_corelcd.setBrightness(0.5f);

	_coredsi.enableTearingEffectReporting();
}

void CoreVideo::turnOff()
{
	_corelcd.turnOff();
}

void CoreVideo::turnOn()
{
	_corelcd.turnOn();
}

void CoreVideo::setBrightness(float value)
{
	_corelcd.setBrightness(value);
}

void CoreVideo::displayImage(LKCoreFatFs &file)
{
	_corejpeg.decodeImage(file);

	auto config = _corejpeg.getConfig();

	_coredma2d.transferImage(config.ImageWidth, config.ImageHeight, CoreJPEG::getWidthOffset(config));
}

void CoreVideo::displayVideo(LKCoreFatFs &file)
{
	uint32_t frame_index  = 0;
	uint32_t frame_size	  = 0;
	uint32_t frame_offset = CoreJPEG::findFrameOffset(file, 0);
	JPEG_ConfTypeDef config;

	while (frame_offset != 0) {
		file.seek(frame_offset);

		frame_size = _corejpeg.decodeImage(file);

		// if first frame, get file info
		if (frame_index == 0) {
			config = _corejpeg.getConfig();
		}

		frame_index += 1;

		_coredma2d.transferImage(config.ImageWidth, config.ImageHeight, CoreJPEG::getWidthOffset(config));

		// get next frame offset
		frame_offset = CoreJPEG::findFrameOffset(file, frame_offset + frame_size + 4);

		display();
	}
	log_info("%d frames", frame_index);
}

void CoreVideo::display()
{
	static auto start_time = rtos::Kernel::Clock::now();

	// wait for DMA2D to finish transfer
	while (_coredma2d.getHandle().State != HAL_DMA2D_STATE_READY)
		;

	// refresh screen
	_coredsi.refresh();

	// wait for DSI to finish refresh
	while (_coredsi.getHandle().State != HAL_DSI_STATE_READY || _coredsi.isBusy())
		;

	// cap fps
	auto dt = rtos::Kernel::Clock::now() - start_time;
	if (dt < 40ms) {
		rtos::ThisThread::sleep_for(40ms - dt);
	}

	log_info("(%ld) %ld ms = %f fps", dt, 1000.f / dt.count());
	start_time = rtos::Kernel::Clock::now();
}
