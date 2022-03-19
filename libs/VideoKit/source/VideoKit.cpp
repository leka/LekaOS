// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "VideoKit.h"

using namespace leka;
using namespace std::chrono_literals;

constexpr uint32_t START_VIDEO_FLAG(1UL << 1);
constexpr uint32_t STOP_VIDEO_FLAG(1UL << 2);
constexpr uint32_t END_OF_VIDEO_FLAG(1UL << 3);
constexpr uint32_t PLAY_ONCE_VIDEO_FLAG(1UL << 4);

VideoKit::VideoKit(interface::STM32Hal &hal)
	: _hal(hal),
	  // peripherals
	  _coresdram(_hal),
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

void VideoKit::initializeScreen()
{
	_coresdram.initialize();

	initialize();
	turnOff();
	setFrameRateLimit(30);

	_video_thread.start(mbed::Callback(this, &VideoKit::runVideo));
}

void VideoKit::turnOn()
{
	_corelcd.turnOn();
	setBrightness(1.F);
}

void VideoKit::turnOff()
{
	_corelcd.turnOff();
	setBrightness(0.F);
}

void VideoKit::displayImage(const char *path)
{
	auto image = gfx::Image {path};

	rtos::ThisThread::sleep_for(100ms);
	stopVideo();
	rtos::ThisThread::sleep_for(100ms);

	draw(image);
	display();
}

void VideoKit::playVideo(const char *path, bool must_loop)
{
	rtos::ThisThread::sleep_for(100ms);
	stopVideo();
	rtos::ThisThread::sleep_for(100ms);

	if (must_loop) {
		_event_flags.clear(PLAY_ONCE_VIDEO_FLAG);
	} else {
		_event_flags.set(PLAY_ONCE_VIDEO_FLAG);
	}

	_full_path_video = path;
	_event_flags.set(START_VIDEO_FLAG);
};

void VideoKit::stopVideo()
{
	_event_flags.set(STOP_VIDEO_FLAG);
};

void VideoKit::runVideo()
{
	auto hasToStopVideo		= [&]() { return (_event_flags.get() & STOP_VIDEO_FLAG) == STOP_VIDEO_FLAG; };
	auto hasToPlayOnceVideo = [&]() { return (_event_flags.get() & PLAY_ONCE_VIDEO_FLAG) == PLAY_ONCE_VIDEO_FLAG; };

	while (true) {
		_event_flags.wait_any(START_VIDEO_FLAG);

		gfx::Video video(_full_path_video);

		video.restart();
		_event_flags.clear(STOP_VIDEO_FLAG | END_OF_VIDEO_FLAG);
		while (!hasToStopVideo() && !video.hasEnded()) {
			draw(video);
			video.nextFrame();
			display();
			turnOn();
		}
		if (video.hasEnded()) {
			_event_flags.set(END_OF_VIDEO_FLAG);
			rtos::ThisThread::sleep_for(300ms);
			if (!hasToPlayOnceVideo() && !hasToStopVideo()) {
				_event_flags.set(START_VIDEO_FLAG);
			}
		}
	}
};

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

void VideoKit::setBrightness(float value)
{
	_corelcd.setBrightness(value);
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

void VideoKit::fillJPEGConfig(interface::File &file, JPEGConfig *config)
{
	_corejpeg.decodeImage(file);
	auto c	= _corejpeg.getConfig();
	*config = c;
}

void VideoKit::clear(gfx::Color color)
{
	_coredma2d.fillRect(0, 0, lcd::dimension::width, lcd::dimension::height, color.toARGB8888());
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
	auto size			  = strlen(text);
	auto foreground_color = CGColor {color.r, color.g, color.b, color.a};
	auto background_color = CGColor {bg_color.r, bg_color.g, bg_color.b, bg_color.a};

	auto ll	   = CoreLL {};
	auto pixel = CGPixel {ll};
	auto font  = CoreFont {pixel};

	font.display(text, size, static_cast<int>(y / graphics::font_pixel_height), foreground_color, background_color);
}

auto VideoKit::drawImage(interface::File &file) -> uint32_t
{
	auto img_size = _corejpeg.decodeImage(file);

	auto c = _corejpeg.getConfig();
	_coredma2d.transferImage(c.ImageWidth, c.ImageHeight, c.getWidthOffset());

	return img_size;
}

auto VideoKit::drawImage(interface::File &file, JPEGConfig &config) -> uint32_t
{
	auto img_size = _corejpeg.decodeImage(file);

	_coredma2d.transferImage(config.ImageWidth, config.ImageHeight, config.getWidthOffset());

	return img_size;
}

void VideoKit::display()
{
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
	// printf("Frame loaded in %lld ms = %.3f fps\n", dt.count(), (1000.f / dt.count()));

	_whole_duration += dt;
	_whole_tick += 1;
	if (dt > _maximum) {
		// printf("Frame loaded in %lld ms = %.3f fps\n", dt.count(), (1000.f / dt.count()));
		_maximum = dt;
	}

	_last_time = rtos::Kernel::Clock::now();
}

void VideoKit::resetCounters()
{
	_whole_duration = rtos::Kernel::Clock::now() - rtos::Kernel::Clock::now();
	_whole_tick		= 0;
	_maximum		= rtos::Kernel::Clock::now() - rtos::Kernel::Clock::now();
}

void VideoKit::displayCounters()
{
	printf("Duration: %lld ms with %ld frames - Mean is %.3f fps\n", _whole_duration.count(), _whole_tick,
		   1000.f * _whole_tick / _whole_duration.count());
	printf("Minimal fps is %.3f fps\n\n", (1000.f / _maximum.count()));
}
