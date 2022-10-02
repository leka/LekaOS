// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/DMA2D.hpp"
#include "interface/DSI.hpp"
#include "interface/Font.hpp"
#include "interface/Graphics.hpp"
#include "interface/JPEG.hpp"
#include "interface/LTDC.hpp"
#include "interface/SDRAM.hpp"
#include "interface/drivers/LCD.hpp"
#include "interface/drivers/STM32Hal.h"
#include "interface/drivers/Video.h"
#include "interface/platform/File.h"

namespace leka {

class CoreVideo : public interface::Video
{
  public:
	CoreVideo(interface::STM32Hal &hal, interface::SDRAM &coresdram, interface::DMA2DBase &coredma2d,
			  interface::DSIBase &coredsi, interface::LTDCBase &coreltdc, interface::LCD &corelcd,
			  interface::Graphics &coregraphics, interface::Font &corefont, interface::JPEGBase &corejpeg);

	void initialize() final;

	void turnOff() final;
	void turnOn() final;

	void setBrightness(float value) final;

	void clearScreen() final;
	void clearScreen(CGColor color);
	void displayRectangle(interface::Graphics::FilledRectangle rectangle, CGColor color);
	void displayImage(interface::File &file, JPEGImageProperties *image_properties = nullptr) final;

	void setVideo(interface::File &file) final;
	void displayNextFrameVideo(interface::File &file) final;
	auto isLastFrame() -> bool final;

	void displayText(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground = CGColor::black,
					 CGColor background = CGColor::white);

	auto getDMA2DHandle() -> DMA2D_HandleTypeDef &;
	auto getLTDCHandle() -> LTDC_HandleTypeDef &;
	auto getJPEGHandle() -> JPEG_HandleTypeDef &;

  private:
	interface::STM32Hal &_hal;
	interface::SDRAM &_coresdram;
	interface::DMA2DBase &_coredma2d;
	interface::DSIBase &_coredsi;
	interface::LTDCBase &_coreltdc;
	interface::LCD &_corelcd;
	interface::Graphics &_coregraphics;
	interface::Font &_corefont;
	interface::JPEGBase &_corejpeg;

	size_t _image_size {0};
	JPEGImageProperties _image_properties {};
	size_t _frame_index {0};
	bool _is_last_frame {false};
};

#define HAL_VIDEO_DECLARE_IRQ_HANDLERS(instance)                                                                       \
	extern "C" {                                                                                                       \
	void DMA2D_IRQHandler(void)                                                                                        \
	{                                                                                                                  \
		HAL_DMA2D_IRQHandler(&(instance).getDMA2DHandle());                                                            \
	}                                                                                                                  \
	void LTDC_IRQHandler(void)                                                                                         \
	{                                                                                                                  \
		HAL_LTDC_IRQHandler(&(instance).getLTDCHandle());                                                              \
	}                                                                                                                  \
	void JPEG_IRQHandler(void)                                                                                         \
	{                                                                                                                  \
		HAL_JPEG_IRQHandler(&(instance).getJPEGHandle());                                                              \
	}                                                                                                                  \
	void DMA2_Stream0_IRQHandler(void)                                                                                 \
	{                                                                                                                  \
		HAL_DMA_IRQHandler((instance).getJPEGHandle().hdmain);                                                         \
	}                                                                                                                  \
	void DMA2_Stream1_IRQHandler(void)                                                                                 \
	{                                                                                                                  \
		HAL_DMA_IRQHandler((instance).getJPEGHandle().hdmaout);                                                        \
	}                                                                                                                  \
	}

}	// namespace leka
