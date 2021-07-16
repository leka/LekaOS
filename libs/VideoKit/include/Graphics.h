#ifndef _LEKA_OS_LIB_GRAPHICS_H
#define _LEKA_OS_LIB_GRAPHICS_H

#include <cstdint>

#include "CoreJPEG.h"
#include "LKCoreFatFs.h"

namespace leka {
class VideoKit;
}

namespace leka::gfx {

struct Color {
	uint8_t r = 0x00;
	uint8_t g = 0x00;
	uint8_t b = 0x00;
	uint8_t a = 0xff;

	auto toARGB8888() -> uint32_t;

	static Color White;
	static Color Black;
	static Color Red;
	static Color Green;
	static Color Blue;
	static Color Yellow;
	static Color Cyan;
	static Color Magenta;
};

struct Rectangle {
	uint32_t width;
	uint32_t height;
	Color color;
};

class Drawable
{
	friend VideoKit;
	virtual void draw(VideoKit &screen) = 0;
};

class Image : public Drawable
{
  public:
	Image(const char *path);

  private:
	void draw(VideoKit &screen) final;
	LKCoreFatFs _file;
};

class Video : public Drawable
{
  public:
	Video(const char *path);
	auto hasEnded() -> bool;
	void restart();

	auto getProgress() -> float;

  private:
	void draw(VideoKit &screen) final;
	LKCoreFatFs _file;
	JPEG_ConfTypeDef _config;

	uint32_t _frame_index  = 0;
	uint32_t _frame_offset = 0;

	bool _ended = false;
};

}	// namespace leka::gfx

#endif	 // _LEKA_OS_LIB_GRAPHICS_H