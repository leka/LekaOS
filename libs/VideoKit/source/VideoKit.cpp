// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "VideoKit.h"

#include "LogKit.h"

using namespace leka;

VideoKit::VideoKit()
	: _coresdram(_hal),
	  // peripherals
	  _corejpeg(_hal, std::make_unique<CoreJPEGDMAMode>()),
	  _coredma2d(_hal),
	  // screen + dsi + ltdc
	  _coreltdc(_hal),
	  _coredsi(_hal, _coreltdc),
	  _coreotm(_coredsi, PinName::SCREEN_BACKLIGHT_PWM),
	  _corelcd(_coreotm)
{
}

void VideoKit::initialize()
{
	__HAL_RCC_LTDC_CLK_ENABLE();

	__HAL_RCC_LTDC_FORCE_RESET();
	__HAL_RCC_LTDC_RELEASE_RESET();

	__HAL_RCC_DMA2D_CLK_ENABLE();

	__HAL_RCC_DMA2D_FORCE_RESET();
	__HAL_RCC_DMA2D_RELEASE_RESET();

	__HAL_RCC_DSI_CLK_ENABLE();

	__HAL_RCC_DSI_FORCE_RESET();
	__HAL_RCC_DSI_RELEASE_RESET();

	HAL_NVIC_SetPriority(LTDC_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(LTDC_IRQn);

	HAL_NVIC_SetPriority(DMA2D_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(DMA2D_IRQn);

	HAL_NVIC_SetPriority(DSI_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(DSI_IRQn);

	_coresdram.initialize();

	_corejpeg.initialize();
	_coredma2d.initialize();

	_coreltdc.initialize();

	_coredsi.initialize();

	_corelcd.initialize();
	_corelcd.setBrightness(0.5f);

	_coredsi.enableTearingEffectReporting();
}

auto VideoKit::getDSI() -> CoreDSI &
{
	return _coredsi;
}

auto VideoKit::getLTDC() -> CoreLTDC &
{
	return _coreltdc;
}

auto VideoKit::getDMA2D() -> CoreDMA2D &
{
	return _coredma2d;
}

auto VideoKit::getJPEG() -> CoreJPEG &
{
	return _corejpeg;
}

void VideoKit::setFrameRateLimit(unsigned framerate)
{
	_frametime = (1000ms / framerate);
}

void VideoKit::clear(gfx::Color color)
{
	_coredma2d.fillRect(0, 0, lcd::dimension::width, lcd::dimension::height, color.toARGB8888());
}

void VideoKit::drawRectangle(gfx::Rectangle rect, uint32_t x, uint32_t y)
{
	_coredma2d.fillRect(x, y, rect.width, rect.height, rect.color.toARGB8888());
}

void VideoKit::drawImage(interface::File &file)
{
	file.seek(0, SEEK_SET);
	_corejpeg.decodeImage(file);

	auto config = _corejpeg.getConfig();

	_coredma2d.transferImage(config.ImageWidth, config.ImageHeight, CoreJPEG::getWidthOffset(config));
}

void VideoKit::drawVideo(interface::File &file)
{
	uint32_t frame_index  = 0;
	uint32_t frame_size	  = 0;
	uint32_t frame_offset = CoreJPEG::findFrameOffset(file, 0);
	JPEG_ConfTypeDef config;

	while (frame_offset != 0) {
		file.seek(frame_offset, SEEK_SET);

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

void VideoKit::display()
{
	// wait for DMA2D to finish transfer
	while (_coredma2d.getHandle().State != HAL_DMA2D_STATE_READY)
		;

	refresh();
	tick();

	// wait for DSI to finish refresh before starting the new frame
	while (_coredsi.isBusy())
		;
}

void VideoKit::refresh()
{
	_coredsi.refresh();
}

void VideoKit::tick()
{
	auto dt = rtos::Kernel::Clock::now() - _last_time;

	if (dt < _frametime) {
		rtos::ThisThread::sleep_for(_frametime - dt);
	}

	dt = rtos::Kernel::Clock::now() - _last_time;
	log_info("(%ld) %ld ms = %f fps", dt.count(), 1000.f / dt.count());

	_last_time = rtos::Kernel::Clock::now();
}
