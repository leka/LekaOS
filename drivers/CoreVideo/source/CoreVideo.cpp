// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreVideo.hpp"

#include "LogKit.h"

using namespace leka;

CoreVideo::CoreVideo(interface::STM32Hal &hal, interface::SDRAM &coresdram, interface::DMA2DBase &coredma2d,
					 interface::DSIBase &coredsi, interface::LTDCBase &coreltdc, interface::LCD &corelcd,
					 interface::Graphics &coregraphics, interface::Font &corefont, interface::JPEGBase &corejpeg)
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

void CoreVideo::initialize()
{
	/** @brief Enable the LTDC clock */
	_hal.HAL_RCC_LTDC_CLK_ENABLE();

	/** @brief Toggle Sw reset of LTDC IP */
	_hal.HAL_RCC_LTDC_FORCE_RESET();
	_hal.HAL_RCC_LTDC_RELEASE_RESET();

	_coredsi.initialize();

	_coreltdc.initialize();

	_coredsi.start();

	_coresdram.initialize();

	_corelcd.initialize();
	_corejpeg.initialize();
	_coredma2d.initialize();

	_corelcd.setBrightness(0.5F);
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

void CoreVideo::clearScreen(CGColor color)
{
	_coregraphics.clearScreen(color);
}

void CoreVideo::displayRectangle(interface::Graphics::FilledRectangle rectangle, CGColor color)
{
	_coregraphics.drawRectangle(rectangle, color);
}

void CoreVideo::displayImage(interface::File &file)
{
	_corejpeg.decodeImage(file);

	auto config = _corejpeg.getConfig();

	_coredma2d.transferImage(config.ImageWidth, config.ImageHeight, CoreJPEG::getWidthOffset(config));
}

void CoreVideo::displayVideo(interface::File &file)
{
	uint32_t frame_index  = 0;
	uint32_t frame_size	  = 0;
	uint32_t frame_offset = CoreJPEG::findFrameOffset(file, 0);
	JPEG_ConfTypeDef config;

	while (frame_offset != 0) {
		file.seek(frame_offset, SEEK_SET);

		auto start_time = HAL_GetTick();
		frame_size		= _corejpeg.decodeImage(file);

		// if first frame, get file info
		if (frame_index == 0) config = _corejpeg.getConfig();

		frame_index += 1;

		_coredma2d.transferImage(config.ImageWidth, config.ImageHeight, CoreJPEG::getWidthOffset(config));

		// get next frame offset
		frame_offset = CoreJPEG::findFrameOffset(file, frame_offset + frame_size + 4);

		auto dt = HAL_GetTick() - start_time;
		if (dt < 1000.f / 25.f) HAL_Delay(1000.f / 25.f - dt);

		std::array<char, 32> buff;
		sprintf(buff.data(), "%3lu ms = %5.2f fps", dt, 1000.f / dt);
		// displayText(buff, strlen(buff), 20);
		log_info("%s", buff.data());
	}
	log_info("%d frames", frame_index);
}

void CoreVideo::displayText(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground,
							CGColor background)
{
	_corefont.display(text, size, starting_line, foreground, background);
}
