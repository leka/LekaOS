#include "Graphics.h"

#include "VideoKit.h"

using namespace leka;
using namespace leka::gfx;

// --- gfx::Color ---------------------------------------
auto Color::toARGB8888() -> uint32_t
{
	return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}

Color Color::White	 = {0xff, 0xff, 0xff};
Color Color::Black	 = {0x00, 0x00, 0x00};
Color Color::Red	 = {0xff, 0x00, 0x00};
Color Color::Green	 = {0x00, 0xff, 0x00};
Color Color::Blue	 = {0x00, 0x00, 0xff};
Color Color::Yellow	 = {0xff, 0xff, 0x00};
Color Color::Cyan	 = {0x00, 0xff, 0xff};
Color Color::Magenta = {0xff, 0x00, 0xff};

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

// --- gfx::Image -------------------------------------
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
	screen.getJPEG().decodeImage(_file);

	auto config = screen.getJPEG().getConfig();
	screen.getDMA2D().transferImage(config.ImageWidth, config.ImageHeight, config.getWidthOffset());
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
	nextFrame();
}

void Video::draw(VideoKit &screen)
{
	// decode current frame
	auto frame_size = screen.getJPEG().decodeImage(_file);

	// get configuration on first frame
	if (!_config.initialized) {
		_config = screen.getJPEG().getConfig();
	}

	// transfer frame to frame buffer
	screen.getDMA2D().transferImage(_config.ImageWidth, _config.ImageHeight, _config.getWidthOffset());

	// increment frame offset
	if (!_ended) {
		_frame_offset = _frame_offset + frame_size + 4;
	}
}
