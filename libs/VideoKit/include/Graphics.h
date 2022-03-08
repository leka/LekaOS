#ifndef _LEKA_OS_LIB_GRAPHICS_H
#define _LEKA_OS_LIB_GRAPHICS_H

#include <cstdint>

#include "rtos/Kernel.h"

#include "CoreJPEG.hpp"
#include "FileSystemKit.h"

namespace leka {
class VideoKit;
}

namespace leka::gfx {

struct Color {
	uint8_t r = 0x00;
	uint8_t g = 0x00;
	uint8_t b = 0x00;
	uint8_t a = 0xff;

	inline constexpr auto toARGB8888() -> uint32_t { return (a << 24) | (r << 16) | (g << 8) | (b << 0); };

	static const Color Transparent;
	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
	static const Color Cyan;
	static const Color Magenta;
};

class Drawable
{
  public:
	virtual ~Drawable() = default;

  private:
	friend VideoKit;
	virtual void draw(VideoKit &screen) = 0;
};

class Rectangle : public Drawable
{
  public:
	Rectangle(uint32_t w, uint32_t h, Color color = Color::White);
	Rectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, Color color = Color::White);

	uint32_t x;
	uint32_t y;
	uint32_t width;
	uint32_t height;
	Color color;

  private:
	void draw(VideoKit &screen) final;
};

class Text : public Drawable
{
  public:
	Text(const char *string, uint32_t x, uint32_t y, Color color, Color bg = Color::Transparent);

	const char *string;
	uint32_t x;
	uint32_t y;
	Color color;
	Color bg;

  private:
	void draw(VideoKit &screen) final;
};

class Image : public Drawable
{
  public:
	Image(const char *path);
	~Image();

  private:
	void draw(VideoKit &screen) final;
	FileSystemKit::File _file;
};

class Video : public Drawable
{
  public:
	Video(const char *path);
	~Video();

	void nextFrame();

	auto getTime() -> int64_t;
	auto getProgress() -> float;

	auto hasEnded() -> bool;
	void restart();

  private:
	void draw(VideoKit &screen) final;

	FileSystemKit::File _file;
	JPEGConfig _config;

	uint32_t _frame_index  = 0;
	uint32_t _frame_offset = 0;

	rtos::Kernel::Clock::time_point _start_time;

	bool _ended = false;
};

}	// namespace leka::gfx

#endif	 // _LEKA_OS_LIB_GRAPHICS_H
