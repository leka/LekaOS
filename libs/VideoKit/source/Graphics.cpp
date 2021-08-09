#include "Graphics.h"

#include "VideoKit.h"

using namespace leka;
using namespace leka::gfx;

// --- gfx::Color ---------------------------------------
const Color Color::Transparent = {0x00, 0x00, 0x00, 0x00};
const Color Color::Black	   = {0x00, 0x00, 0x00};
const Color Color::White	   = {0xff, 0xff, 0xff};
const Color Color::Red		   = {0xff, 0x00, 0x00};
const Color Color::Green	   = {0x00, 0xff, 0x00};
const Color Color::Blue		   = {0x00, 0x00, 0xff};
const Color Color::Yellow	   = {0xff, 0xff, 0x00};
const Color Color::Cyan		   = {0x00, 0xff, 0xff};
const Color Color::Magenta	   = {0xff, 0x00, 0xff};

// --- gfx::Rectangle -----------------------------------
Rectangle::Rectangle(uint32_t w, uint32_t h, Color color) : Rectangle(0, 0, w, h, std::move(color)) {}

Rectangle::Rectangle(uint32_t posx, uint32_t posy, uint32_t w, uint32_t h, Color col)
	: x(posx), y(posy), width(w), height(h), color(std::move(col))
{
}

void Rectangle::draw(VideoKit &screen)
{
	screen.drawRectangle(x, y, width, height, color);
}

// --- gfx::Text ----------------------------------------

Text::Text(const char *text, uint32_t posx, uint32_t posy, Color col, Color bg_col)
	: string(std::move(text)), x(posx), y(posy), color(std::move(col)), bg(std::move(bg_col))
{
}

void Text::draw(VideoKit &screen)
{
	screen.drawText(string, x, y, color, bg);
}

// --- gfx::Image ---------------------------------------
Image::Image(const char *path)
{
	_file.open(path);
}

Image::~Image()
{
	_file.close();
}

void Image::draw(VideoKit &screen)
{
	_file.seek(0);
	screen.drawImage(_file);
}

// --- gfx::Video -------------------------------------
Video::Video(const char *path)
{
	_file.open(path);
	restart();
}

Video::~Video()
{
	_file.close();
}

void Video::nextFrame()
{
	auto offset = CoreJPEG::findFrameOffset(_file, _frame_offset);
	if (offset != 0) {
		_frame_offset = offset;
		_file.seek(_frame_offset);
		_frame_index += 1;
	} else {
		_ended = true;
	}
}

auto Video::getTime() -> int64_t
{
	return (rtos::Kernel::Clock::now() - _start_time).count();
}

auto Video::getProgress() -> float
{
	auto progress = float(_frame_offset) / _file.getSize();
	// clamp progress to be in range [0.f, 1.f]
	return progress > 1.f ? 1.f : progress < 0.f ? 0.f : progress;
}

auto Video::hasEnded() -> bool
{
	return _ended;
}

void Video::restart()
{
	_file.seek(0);
	_frame_offset = 0;
	_frame_index  = 0;
	_ended		  = false;
	_start_time	  = rtos::Kernel::Clock::now();
	nextFrame();
}

void Video::draw(VideoKit &screen)
{
	// get configuration on first frame
	if (!_config.initialized) {
		screen.fillConfig(_file, &_config);
		restart();
	}

	uint32_t frame_size = screen.drawImage(_file, _config);
	// increment frame offset
	if (!_ended) {
		_frame_offset = _frame_offset + frame_size + 4;
	}
}
