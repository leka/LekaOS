// Leka - LekaOS
// Copyright 2020 APF France handicap & FastLED (https://github.com/FastLED/FastLED)
// SPDX-License-Identifier: MIT

#ifndef _LK_ALPHA_OS_LIB_LED_SK9822_CONTROLLER_H_
#define _LK_ALPHA_OS_LIB_LED_SK9822_CONTROLLER_H_

#include "FastLED.h"
#include "pixeltypes.h"

///
/// @brief SK9822 controller class.
///		   Based on https://github.com/FastLED/FastLED/blob/master/chipsets.h
///
/// @tparam DATA_PIN the data pin for these leds
/// @tparam CLOCK_PIN the clock pin for these leds
/// @tparam RGB_ORDER the RGB ordering for these leds
/// @tparam SPI_SPEED the clock divider used for these leds. Set using the DATA_RATE_MHZ/DATA_RATE_KHZ macros.
/// Defaults to DATA_RATE_MHZ(24)
///
template <uint8_t DATA_PIN, uint8_t CLOCK_PIN, EOrder RGB_ORDER = RGB, uint32_t SPI_SPEED = DATA_RATE_MHZ(24)>
class SK9822Controller : public CPixelLEDController<RGB_ORDER>
{
  public:
	SK9822Controller() {}

	virtual void init() { mSPI.init(); }

  protected:
	virtual void showPixels(PixelController<RGB_ORDER> &pixels)
	{
		mSPI.select();

		uint8_t s0 = pixels.getScale0(), s1 = pixels.getScale1(), s2 = pixels.getScale2();

		const uint16_t maxBrightness = 0x1F;
		uint16_t brightness			 = ((((uint16_t)max(max(s0, s1), s2) + 1) * maxBrightness - 1) >> 8) + 1;
		s0							 = (maxBrightness * s0 + (brightness >> 1)) / brightness;
		s1							 = (maxBrightness * s1 + (brightness >> 1)) / brightness;
		s2							 = (maxBrightness * s2 + (brightness >> 1)) / brightness;

		startBoundary();
		while (pixels.has(1)) {
			writeLed(brightness, pixels.loadAndScale0(0, s0), pixels.loadAndScale1(0, s1), pixels.loadAndScale2(0, s2));
			pixels.stepDithering();
			pixels.advanceData();
		}

		endBoundary(pixels.size());

		mSPI.waitFully();
		mSPI.release();
	}

  private:
	// TODO: move to mbed SPI
	typedef SPIOutput<DATA_PIN, CLOCK_PIN, SPI_SPEED> SPI;
	SPI mSPI;

	void startBoundary()
	{
		mSPI.writeWord(0);
		mSPI.writeWord(0);
	}
	void endBoundary(int nLeds)
	{
		int nLongWords = (nLeds / 32);
		do {
			mSPI.writeByte(0x00);
			mSPI.writeByte(0x00);
			mSPI.writeByte(0x00);
			mSPI.writeByte(0x00);
		} while (nLongWords--);
	}

	inline void writeLed(uint8_t brightness, uint8_t b0, uint8_t b1, uint8_t b2) __attribute__((always_inline))
	{
		// #ifdef FASTLED_SPI_BYTE_ONLY
		mSPI.writeByte(0xE0 | brightness);
		mSPI.writeByte(b0);
		mSPI.writeByte(b1);
		mSPI.writeByte(b2);
		// #else
		// 		uint16_t b = 0xE000 | (brightness << 8) | (uint16_t)b0;
		// 		mSPI.writeWord(b);
		// 		uint16_t w = b1 << 8;
		// 		w |= b2;
		// 		mSPI.writeWord(w);
		// #endif
	}
};	 // _LK_ALPHA_OS_LIB_LED_SK9822_CONTROLLER_H_
