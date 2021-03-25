// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_FONT_MOCK_H_
#define _LEKA_OS_FONT_MOCK_H_

#include "LKCoreFontBase.h"
#include "gmock/gmock.h"

namespace leka {

class LKCoreFontMock : public LKCoreFontBase
{
  public:
	MOCK_METHOD(void, draw, (Character character, Color foreground, Color background), (override));
	MOCK_METHOD(void, display,
				(const char *text, uint32_t size, uint32_t starting_line, Color foreground, Color background),
				(override));
	MOCK_METHOD(const uint8_t *, fontGetFirstPixelAddress, (char character), (override));
	MOCK_METHOD(uint32_t, fontGetPixelBytes, (uint8_t * line_address), (override));
	MOCK_METHOD(bool, fontPixelIsOn, (uint32_t byte_of_line, uint8_t pixel_id), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_FONT_MOCK_H_
