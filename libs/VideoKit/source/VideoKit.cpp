#include <functional>

#include "VideoKit.h"

using namespace leka;
using namespace std::chrono_literals;

VideoKit::VideoKit(LKCoreSTM32HalBase &hal)
	: _hal(hal),
	  // peripherals
	  _corejpegmode(_hal),
	  _corejpeg(_hal, _corejpegmode),
	  _coredma2d(_hal),
	  // screen + dsi + ltdc
	  _coreltdc(_hal),
	  _coredsi(_hal, _coreltdc),
	  _coreotm(_coredsi, PinName::SCREEN_BACKLIGHT_PWM),
	  _corelcd(_coreotm),
	  _frametime(40ms)
{
}

void VideoKit::initialize()
{
	_corejpeg.initialize();
	_coredma2d.initialize();

	_coreltdc.initialize();

	_coredsi.initialize();

	_corelcd.initialize();
	_corelcd.setBrightness(1.f);

	if (dsi::refresh_columns_count > 1) {
		_coredsi.enableTearingEffectReporting();
	}

	_last_time = rtos::Kernel::Clock::now();
}

auto VideoKit::getDSI() -> CoreDSI &
{
	return _coredsi;
}

auto VideoKit::getLTDC() -> CoreLTDC &
{
	return _coreltdc;
}

auto VideoKit::getDMA2D() -> CoreDMA2D &
{
	return _coredma2d;
}

auto VideoKit::getJPEG() -> CoreJPEG &
{
	return _corejpeg;
}

void VideoKit::setFrameRateLimit(uint32_t framerate)
{
	_frametime = (1000ms / framerate);
}

void VideoKit::fillJPEGConfig(FileSystemKit::File &file, JPEGConfig *config)
{
	_corejpeg.decodeImage(file);
	auto c	= _corejpeg.getConfig();
	*config = c;
}

void VideoKit::clear(gfx::Color color)
{
	_coredma2d.fillRect(0, 0, lcd::dimension.width, lcd::dimension.height, color.toARGB8888());
}

void VideoKit::draw(gfx::Drawable &drawable)
{
	drawable.draw(*this);
}

void VideoKit::drawRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, gfx::Color color)
{
	_coredma2d.fillRect(x, y, w, h, color.toARGB8888());
}

void VideoKit::drawText(const char *text, uint32_t x, uint32_t y, gfx::Color color, gfx::Color bg_color)
{
	auto posx = x;
	auto posy = y;
	while (*text != '\0') {
		char letter = *text++;
		auto *addr	= Font::getCharAddress(letter);
		for (size_t j = 0; j < Font::character_height; ++j) {
			for (size_t i = 0; i < Font::character_width; ++i) {
				if (Font::isPixelOn(addr, i, j)) {
					_coredma2d.setPixel(posx + i, posy + j, color.toARGB8888());
				} else if (bg_color.a > 0) {
					_coredma2d.setPixel(posx + i, posy + j, bg_color.toARGB8888());
				}
			}
		}
		if (posx > lcd::dimension.width - Font::character_width) {
			posx = x;
			posy += Font::character_height - 2;
		} else {
			posx += Font::character_width - 1;
		}
	}
}

auto VideoKit::drawImage(FileSystemKit::File &file) -> uint32_t
{
	auto img_size = _corejpeg.decodeImage(file);

	auto c = _corejpeg.getConfig();
	_coredma2d.transferImage(c.ImageWidth, c.ImageHeight, c.getWidthOffset());

	return img_size;
}

auto VideoKit::drawImage(FileSystemKit::File &file, JPEGConfig &config) -> uint32_t
{
	auto img_size = _corejpeg.decodeImage(file);

	_coredma2d.transferImage(config.ImageWidth, config.ImageHeight, config.getWidthOffset());

	return img_size;
}

void VideoKit::drawImageAsync(LKCoreFatFs &file, JPEGConfig &config, std::function<void(int)>& cb)
{
	auto videoKitCallbackWrapper = [this, &config, cb] (int img_size) {
        cb(img_size);
		_coredma2d.transferImage(config.ImageWidth, config.ImageHeight, config.getWidthOffset());
        this->async_count--;
    };
    _corejpeg.decodeImageAsync(file, videoKitCallbackWrapper);
	async_count++;
}

void VideoKit::display()
{
	while (async_count != 0) {
		log_info("waiting %d threads", async_count);
		//rtos::ThisThread::yield();
	}
	// wait for DMA2D to finish transfer
	while (!_coredma2d.isReady())
		;

	refresh();
	tick();

	// wait for DSI to finish transfer
	while (!_coredsi.isReady())
		;

	// wait for DSI to finish refresh, very slow but provides
	// better quality when clearing full screen or using 4 DSI refresh columns
	// while (!_coredsi.refreshDone())
	//	;
}

void VideoKit::refresh()
{
	_coredsi.refresh();
}

void VideoKit::tick()
{
	auto dt = rtos::Kernel::Clock::now() - _last_time;

	if (dt < _frametime) {
		rtos::ThisThread::sleep_for(_frametime - dt);
	}

	dt = rtos::Kernel::Clock::now() - _last_time;
	printf("[INFO] [VideoKit.cpp:%d] %lld ms = %.3f fps\n\r", __LINE__, dt.count(), (1000.f / dt.count()));

	_last_time = rtos::Kernel::Clock::now();
}
