// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreVideo.hpp"

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

void CoreVideo::clearScreen()
{
	clearScreen(CGColor::white);
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

	auto image_properties = _corejpeg.getImageProperties();

	_coredma2d.transferImage(image_properties.ImageWidth, image_properties.ImageHeight,
							 image_properties.getWidthOffset());
}

void CoreVideo::playVideo(interface::File &file)
{
	auto config = _corejpeg.getImageProperties();

	auto frame_index = _corejpeg.findSOIMarker(file, 0);
	auto frame_size	 = size_t {0};

	while (frame_index != 0) {
		file.seek(frame_index, SEEK_SET);

		frame_size = _corejpeg.decodeImage(file);
		_coredma2d.transferImage(config.ImageWidth, config.ImageHeight, config.getWidthOffset());

		frame_index = _corejpeg.findSOIMarker(file, frame_index + frame_size);
	}
}

void CoreVideo::displayText(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground,
							CGColor background)
{
	_corefont.display(text, size, starting_line, foreground, background);
}

auto CoreVideo::getDMA2DHandle() -> DMA2D_HandleTypeDef &
{
	return _coredma2d.getHandle();
}

auto CoreVideo::getLTDCHandle() -> LTDC_HandleTypeDef &
{
	return _coreltdc.getHandle();
}

auto CoreVideo::getJPEGHandle() -> JPEG_HandleTypeDef &
{
	return _corejpeg.getHandle();
}
