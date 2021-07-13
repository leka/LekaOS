// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <chrono>

#include "rtos/ThisThread.h"

#include "LKCoreVideo.h"
#include "LogKit.h"

using namespace std::chrono;
using namespace leka;

LKCoreVideo::LKCoreVideo(LKCoreSTM32HalBase &hal, LKCoreSDRAMBase &coresdram, LKCoreDMA2DBase &coredma2d,
						 LKCoreDSIBase &coredsi, LKCoreLTDCBase &coreltdc, LKCoreLCDBase &corelcd,
						 LKCoreGraphicsBase &coregraphics, LKCoreFontBase &corefont, LKCoreJPEGBase &corejpeg)
	: _hal(hal),
	  _coresdram(coresdram),
	  _coredma2d(coredma2d),
	  _coredsi(coredsi),
	  _coreltdc(coreltdc),
	  _corelcd(corelcd),
	  _coregraphics(coregraphics),
	  _corefont(corefont),
	  _corejpeg(corejpeg)
{
}

void LKCoreVideo::initialize()
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

	//_coredsi.enableTearingEffectReporting();
	//_coredsi.setupPartialRefresh();

	// TODO (@madour) : move to somewhere else
	static auto &self = *this;
	static int active_area = 0;
	static uint8_t pColLeft[]	= {0x00, 0x00, 0x01, 0x8F}; /*   0 -> 399 */
	static uint8_t pColRight[] = {0x01, 0x90, 0x03, 0x1F}; /* 400 -> 799 */
	HAL_DSI_RegisterCallback(&_coredsi.getHandle(), HAL_DSI_ENDOF_REFRESH_CB_ID, [](DSI_HandleTypeDef *hdsi){
		/*__HAL_DSI_WRAPPER_DISABLE(hdsi);
		HAL_LTDC_SetAddress(&self._coreltdc.getHandle(), lcd::frame_buffer_address, 0);
		__HAL_DSI_WRAPPER_ENABLE(hdsi);*/

		/*if (active_area == 1) { // right next
			__HAL_DSI_WRAPPER_DISABLE(hdsi);
			//HAL_LTDC_SetAddress(&self._coreltdc.getHandle(), lcd::frame_buffer_address + 400*4, 0);
			__HAL_DSI_WRAPPER_ENABLE(hdsi);

			//HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, 0x2a, pColRight);
			//HAL_DSI_Refresh(hdsi);
			active_area = 0;
			self._coredma2d.go = true;

		} else if (active_area == 0) { // left next
			__HAL_DSI_WRAPPER_DISABLE(hdsi);
			HAL_LTDC_SetAddress(&self._coreltdc.getHandle(), lcd::frame_buffer_address, 0);
			__HAL_DSI_WRAPPER_ENABLE(hdsi);

			HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, 0x2a, pColLeft);
			active_area = 1;
			self._coredma2d.go = true;
		}*/
	});

	HAL_DSI_RegisterCallback(&_coredsi.getHandle(), HAL_DSI_TEARING_EFFECT_CB_ID, [](DSI_HandleTypeDef *hdsi){
		HAL_DSI_ShortWrite(hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, 0x34, 0x00);
		HAL_DSI_Refresh(hdsi);
	});
}

void LKCoreVideo::turnOff()
{
	_corelcd.turnOff();
}

void LKCoreVideo::turnOn()
{
	_corelcd.turnOn();
}

void LKCoreVideo::setBrightness(float value)
{
	_corelcd.setBrightness(value);
}

void LKCoreVideo::clearScreen(CGColor color)
{
	_coregraphics.clearScreen(color);
}

void LKCoreVideo::displayRectangle(LKCoreGraphicsBase::FilledRectangle rectangle, CGColor color)
{
	_coregraphics.drawRectangle(rectangle, color);
}

void LKCoreVideo::displayImage(LKCoreFatFs &file)
{
	_corejpeg.decodeImage(file);

	auto config = _corejpeg.getConfig();

	_coredma2d.transferImage(config.ImageWidth, config.ImageHeight, LKCoreJPEG::getWidthOffset(config));
}

void LKCoreVideo::displayVideo(LKCoreFatFs &file)
{
	uint32_t frame_index  = 0;
	uint32_t frame_size	  = 0;
	uint32_t frame_offset = LKCoreJPEG::findFrameOffset(file, 0);
	JPEG_ConfTypeDef config;

	while (frame_offset != 0) {
		file.seek(frame_offset);

		auto start_time = rtos::Kernel::Clock::now();
		frame_size		= _corejpeg.decodeImage(file);

		// if first frame, get file info
		if (frame_index == 0) {
			config = _corejpeg.getConfig();
		}

		frame_index += 1;

		_coredma2d.transferImage(config.ImageWidth, config.ImageHeight, LKCoreJPEG::getWidthOffset(config));

		// get next frame offset
		frame_offset = LKCoreJPEG::findFrameOffset(file, frame_offset + frame_size + 4);

		// temporaire
		auto dt = rtos::Kernel::Clock::now() - start_time;
		/*
		if (dt < 40ms) {
			rtos::ThisThread::sleep_for(40ms - dt);
		}
		*/

		std::array<char, 32> buff;
		sprintf(buff.data(), "%3lu ms = %5.2f fps", dt.count(), 1000.f / dt.count());
		// displayText(buff, strlen(buff), 20);
		// log_info("%s", buff.data());
		display();
	}
	log_info("%d frames", frame_index);
}

void LKCoreVideo::displayText(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground,
							  CGColor background)
{
	_corefont.display(text, size, starting_line, foreground, background);
}

void LKCoreVideo::display()
{
	static auto time = HAL_GetTick();
	// wait for DMA2D to finish transfer
	auto time2 = HAL_GetTick();
	while (_coredma2d.getHandle().State != HAL_DMA2D_STATE_READY)
		;
	log_info("%lums", HAL_GetTick() - time2);
	// refresh screen
	_coredsi.refresh();
	// wait for DSI to finish refresh
	/*while(false && (_coredsi.isBusy() || _coredsi.busy ))
		;*/

	log_info("%lums", HAL_GetTick() - time);
	time = HAL_GetTick();
}