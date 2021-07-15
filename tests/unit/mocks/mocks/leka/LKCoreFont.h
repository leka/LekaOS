// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_FONT_MOCK_H_
#define _LEKA_OS_FONT_MOCK_H_

#include "gmock/gmock.h"
#include "interface/LKCoreFont.h"

namespace leka {

class LKCoreFontMock : public interface::LKCoreFont
{
  public:
	MOCK_METHOD(void, drawChar, (Character character, CGColor foreground, CGColor background), (override));
	MOCK_METHOD(void, display,
				(const char *text, uint32_t size, uint32_t starting_line, CGColor foreground, CGColor background),
				(override));
	MOCK_METHOD(const uint8_t *, fontGetFirstPixelAddress, (char character), (override));
	MOCK_METHOD(uint32_t, fontGetPixelBytes, (const uint8_t *line_address), (override));
	MOCK_METHOD(bool, fontPixelIsOn, (uint32_t byte_of_line, uint8_t pixel_id), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_FONT_MOCK_H_
