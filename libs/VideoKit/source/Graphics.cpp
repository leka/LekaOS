#include "Graphics.h"

#include "VideoKit.h"

using namespace leka;
using namespace leka::gfx;

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

Image::Image(const char *path)
{
	_file.open(path);
}

void Image::draw(VideoKit &screen)
{
	_file.seek(0);
	screen.getJPEG().decodeImage(_file);

	auto config = screen.getJPEG().getConfig();
	screen.getDMA2D().transferImage(config.ImageWidth, config.ImageHeight, CoreJPEG::getWidthOffset(config));
}

Video::Video(const char *path)
{
	_file.open(path);
	_frame_offset = CoreJPEG::findFrameOffset(_file, 0);
}

auto Video::hasEnded() -> bool
{
	return _ended;
}

void Video::restart()
{
	_file.seek(0);
	_frame_offset = CoreJPEG::findFrameOffset(_file, 0);
	_frame_index  = 0;
	_ended		  = false;
}

auto Video::getProgress() -> float
{
	return (float(_frame_offset) / _file.getSize());
}

void Video::draw(VideoKit &screen)
{
	if (_frame_offset == 0) return;

	_file.seek(_frame_offset);

	auto frame_size = screen.getJPEG().decodeImage(_file);

	if (_frame_index == 0) {
		_config = screen.getJPEG().getConfig();
	}
	screen.getDMA2D().transferImage(_config.ImageWidth, _config.ImageHeight, CoreJPEG::getWidthOffset(_config));

	_frame_index += 1;
	_frame_offset = CoreJPEG::findFrameOffset(_file, _frame_offset + frame_size + 4);

	if (_frame_offset == 0) _ended = true;
}
