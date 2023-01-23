// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreFont.hpp"

#include "CGFont.hpp"
#include "gtest/gtest.h"
#include "internal/corevideo_config.h"
#include "mocks/leka/CoreLL.h"

using namespace leka;
using ::testing::_;
using ::testing::AnyNumber;

// NOLINTBEGIN(modernize-use-trailing-return-type, misc-non-private-member-variables-in-classes)

class CoreFontTest : public ::testing::Test
{
  protected:
	CoreFontTest() : pixel(llmock), font(pixel) {}

	// void SetUp() override {}
	// void TearDown() override {}

	mock::CoreLLMock llmock;
	CGPixel pixel;
	CoreFont font;
};

TEST_F(CoreFontTest, instantiation)
{
	ASSERT_NE(&font, nullptr);
}

TEST_F(CoreFontTest, fontGetFirstPixelAddressOf$)
{
	constexpr char character = '$';
	constexpr auto index	 = 288;	  // See CoreTable, index is given before start of character

	const auto *expected_address = &CGFontTable.at(index);

	const auto *actual_address = font.fontGetFirstPixelAddress(character);

	// Cannot check address directly. Check bytes from selected character are the same.
	for (uint8_t inc = 0; inc < graphics::bytes_per_char; inc++) {
		ASSERT_EQ(*(expected_address + inc), *(actual_address + inc));
	}
}

TEST_F(CoreFontTest, fontGetFirstPixelAddressOfA)
{
	constexpr char character = 'A';
	constexpr auto index	 = 2376;   // See CoreTable, index is given before start of character

	const auto *expected_address = &CGFontTable.at(index);

	const auto *actual_address = font.fontGetFirstPixelAddress(character);

	// Cannot check address directly. Check bytes from selected character are the same.
	for (uint8_t inc = 0; inc < graphics::bytes_per_char; inc++) {
		ASSERT_EQ(*(expected_address + inc), *(actual_address + inc));
	}
}

TEST_F(CoreFontTest, fontGetFirstPixelAddressOfz)
{
	constexpr char character = 'z';
	constexpr auto index	 = 6480;   // See CoreTable, index is given before start of character

	const auto *expected_address = &CGFontTable.at(index);

	const auto *actual_address = font.fontGetFirstPixelAddress(character);

	// Cannot check address directly. Check bytes from selected character are the same.
	for (uint8_t inc = 0; inc < graphics::bytes_per_char; inc++) {
		ASSERT_EQ(*(expected_address + inc), *(actual_address + inc));
	}
}

TEST_F(CoreFontTest, fontGetPixelBytes)
{
	constexpr auto line_to_convert	   = std::to_array<const uint8_t>({0x2A, 0x2B, 0x2C});
	constexpr auto expected_conversion = 0x002A2B2C;

	auto actual_conversion = font.fontGetPixelBytes(line_to_convert.data());

	ASSERT_EQ(expected_conversion, actual_conversion);
}

TEST_F(CoreFontTest, fontGetPixelBytesFailed)
{
	constexpr auto line_to_convert	   = std::to_array<uint8_t>({0x2A, 0x2B, 0x2C});
	constexpr auto expected_conversion = 0x002A2B2D;   // Note 2D at the end

	auto actual_conversion = font.fontGetPixelBytes(line_to_convert.data());

	ASSERT_NE(expected_conversion, actual_conversion);
}

TEST_F(CoreFontTest, fontPixelIsOnWithPattern)
{
	const uint32_t byte_of_line = 0x00AAAAAA;	// 0xA = 1010

	// Check that odd bits are equal to 0 and even bits are equal to 1
	for (uint8_t pixel_id = 0; pixel_id < graphics::pixels_per_line; pixel_id++) {
		if (pixel_id % 2 == 0) {
			ASSERT_TRUE(font.fontPixelIsOn(byte_of_line, pixel_id));
		} else {
			ASSERT_FALSE(font.fontPixelIsOn(byte_of_line, pixel_id));
		}
	}
}

TEST_F(CoreFontTest, fontPixelIsOnWithC00010FF)
{
	const uint32_t byte_of_line			  = 0xC00010FF;
	const uint8_t max_bit_index_of_a_line = graphics::bytes_per_line * 8 - 1;	// = 23

	for (uint8_t pixel_id = 0; pixel_id < graphics::pixels_per_line; pixel_id++) {
		auto get_bit   = byte_of_line >> (max_bit_index_of_a_line - pixel_id);	 // Read from left to right
		bool bit_is_on = (get_bit & 0x01) != 0;

		ASSERT_EQ(bit_is_on, font.fontPixelIsOn(byte_of_line, pixel_id));
	}
}

TEST_F(CoreFontTest, drawCharacter)
{
	CoreFont::Character character;
	character.ascii = '.';

	auto pixels_per_char = graphics::font_pixel_width * graphics::font_pixel_height;   // 17 * 24 = 408

	EXPECT_CALL(llmock, rawMemoryWrite).Times(pixels_per_char);	  // Expect to draw as much as pixel in any character

	font.drawChar(character);
}

TEST_F(CoreFontTest, drawCharacterWithColor)
{
	// ENHANCEMENT: Set pixels_lit by checking number of bit set.

	CoreFont::Character character;
	character.ascii			 = '.';
	CGColor foreground_color = CGColor::pure_red;
	CGColor background_color = CGColor::black;

	auto pixels_per_char  = graphics::font_pixel_width * graphics::font_pixel_height;	// 17 * 24 = 408
	const auto pixels_lit = 12;	  // Corrolated with LKFontTable for the tested character

	EXPECT_CALL(llmock, rawMemoryWrite(_, foreground_color.getARGB())).Times(pixels_lit);
	EXPECT_CALL(llmock, rawMemoryWrite(_, background_color.getARGB())).Times(pixels_per_char - pixels_lit);

	font.drawChar(character, foreground_color, background_color);
}

TEST_F(CoreFontTest, displayNormalSentence)
{
	auto buff = std::array<char, 128> {};

	auto text_length = snprintf(buff.data(), buff.size(), "Some text");

	const auto starting_line = uint32_t {1};

	CGColor foreground_color = CGColor::black;
	CGColor background_color = CGColor::white;

	auto pixels_per_char	 = graphics::font_pixel_width * graphics::font_pixel_height;   // 17 * 24 = 408
	auto expected_pixel_draw = text_length * pixels_per_char;

	EXPECT_CALL(llmock, rawMemoryWrite).Times(expected_pixel_draw);

	font.display(buff.data(), text_length, starting_line, foreground_color, background_color);
}

TEST_F(CoreFontTest, displayPositiveStartingLine)
{
	auto buff = std::array<char, 128> {};

	auto text_length = snprintf(buff.data(), buff.size(), "Some text");

	const auto starting_line = uint32_t {0};

	EXPECT_CALL(llmock, rawMemoryWrite).Times(0);

	font.display(buff.data(), text_length, starting_line);
}

TEST_F(CoreFontTest, displayExceededStartingLine)
{
	auto buff = std::array<char, 128> {};

	auto text_length = snprintf(buff.data(), buff.size(), "Some text");

	const auto starting_line = uint32_t {50};

	EXPECT_CALL(llmock, rawMemoryWrite).Times(0);

	font.display(buff.data(), text_length, starting_line);
}

TEST_F(CoreFontTest, displayWithNewLine)
{
	// ? # Explanation
	// To check the correct behavior we spy on the coordinates of the last drawn pixel
	// The expected final height is equal to the height of a character on the selected
	// line to which we add the height of a character on the next line once drawn
	// actual_last_drawn_pixel is calculate after we call CoreFont::display(_, _, _).

	// ARRANGE
	auto buff = std::array<char, 128> {};

	const auto starting_line = uint32_t {5};

	auto expected_last_pixel_y_position = graphics::font_pixel_height * starting_line + graphics::font_pixel_height;

	// ? This EXPECT_CALL suppress the GMOCK WARNING: Uninteresting mock function call
	// TODO(@leka/dev-embedded): Remove call in the future
	EXPECT_CALL(llmock, rawMemoryWrite).Times(AnyNumber());

	// ACT
	auto text_length = snprintf(buff.data(), buff.size(), "This is the first line\nThis is the second line");
	font.display(buff.data(), text_length, starting_line);

	// ASSERT
	auto actual_last_drawn_pixel = font.getLastDrawnPixel();
	ASSERT_EQ(actual_last_drawn_pixel.coordinates.y, expected_last_pixel_y_position);
}

TEST_F(CoreFontTest, displayUnwrittableAsciiCharacter)
{
	// ARRANGE
	auto buff = std::array<char, 128> {};

	const auto starting_line = uint32_t {1};

	auto text_length = snprintf(buff.data(), buff.size(), "\tThis is the first line");

	CGPoint expected_last_pixel;
	expected_last_pixel.x = (graphics::font_pixel_width * (text_length - 1)) - 1;
	expected_last_pixel.y = graphics::font_pixel_height;

	// ? This EXPECT_CALL suppress the GMOCK WARNING: Uninteresting mock function call
	// TODO(@leka/dev-embedded): Remove call in the future
	EXPECT_CALL(llmock, rawMemoryWrite).Times(AnyNumber());

	// ACT
	font.display(buff.data(), text_length, starting_line);

	// ASSERT
	auto actual_last_drawn_pixel = font.getLastDrawnPixel();
	ASSERT_EQ(actual_last_drawn_pixel.coordinates.x, expected_last_pixel.x);
	ASSERT_EQ(actual_last_drawn_pixel.coordinates.y, expected_last_pixel.y);
}

TEST_F(CoreFontTest, displayWithScreenWidthReached)
{
	// ARRANGE
	auto buff = std::array<char, 128> {};

	auto text_length =
		snprintf(buff.data(), buff.size(),
				 "This sentence is supposed to be on multiple lines because it is too long to be displayed on "
				 "only one line of the screen.");
	uint8_t max_char_per_line = lcd::dimension::width / graphics::font_pixel_width;
	ASSERT_GT(text_length, max_char_per_line);	 // Text to display MUST exceed 47 characters for this test

	const auto starting_line = uint32_t {1};

	CGPoint expected_last_pixel;
	expected_last_pixel.x = (text_length % max_char_per_line) * graphics::font_pixel_width - 1;
	expected_last_pixel.y = ((text_length / max_char_per_line) + 1) * graphics::font_pixel_height;

	// ? This EXPECT_CALL suppress the GMOCK WARNING: Uninteresting mock function call
	// TODO(@leka/dev-embedded): Remove call in the future
	EXPECT_CALL(llmock, rawMemoryWrite).Times(AnyNumber());

	// ACT
	font.display(buff.data(), text_length, starting_line);

	// ASSERT
	auto actual_last_drawn_pixel = font.getLastDrawnPixel();
	ASSERT_EQ(actual_last_drawn_pixel.coordinates.x, expected_last_pixel.x);
	ASSERT_EQ(actual_last_drawn_pixel.coordinates.y, expected_last_pixel.y);
}

TEST_F(CoreFontTest, displayWithScreenHeightReached)
{
	// ARRANGE
	auto buff = std::array<char, 128> {};

	const auto starting_line = 20;

	auto expected_last_pixel_y_position = lcd::dimension::height;

	// ? This EXPECT_CALL suppress the GMOCK WARNING: Uninteresting mock function call
	// TODO(@leka/dev-embedded): Remove call in the future
	EXPECT_CALL(llmock, rawMemoryWrite).Times(AnyNumber());

	// ACT
	auto text_length = snprintf(buff.data(), buff.size(),
								"This text should appear on the screen\nThis text should NOT appear on the screen");
	font.display(buff.data(), text_length, starting_line);

	// ASSERT
	auto actual_last_drawn_pixel = font.getLastDrawnPixel();
	ASSERT_EQ(actual_last_drawn_pixel.coordinates.y, expected_last_pixel_y_position);
}

// NOLINTEND(modernize-use-trailing-return-type, misc-non-private-member-variables-in-classes)
