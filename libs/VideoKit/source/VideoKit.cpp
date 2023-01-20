// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "VideoKit.h"
#include <mutex>

#include "rtos/Mutex.h"
#include "rtos/ThisThread.h"

#include "FileManagerKit.h"

using namespace leka;
using namespace std::chrono_literals;

namespace {

rtos::Mutex mutex {};

}	// namespace

void VideoKit::initializeScreen()
{
	_video.initialize();

	_video.setBrightness(1.F);
	_video.clearScreen();

	_thread.start([this] { run(); });
}

void VideoKit::displayImage(const std::filesystem::path &path)
{
	const std::scoped_lock lock(mutex);

	auto full_path = _default_image_path / path;

	if (full_path == _current_path) {
		return;
	}

	if (auto file = FileManagerKit::File {full_path}; file.is_open()) {
		_event_flags.set(flags::STOP_VIDEO_FLAG);

		_current_path = full_path;

		rtos::ThisThread::sleep_for(100ms);

		_video.displayImage(file);

		file.close();
	}
}

void VideoKit::fillWhiteBackgroundAndDisplayImage(const std::filesystem::path &path)
{
	const std::scoped_lock lock(mutex);

	auto full_path = _default_image_path / path;

	if (full_path == _current_path) {
		return;
	}

	if (auto file = FileManagerKit::File {full_path}; file.is_open()) {
		_event_flags.set(flags::STOP_VIDEO_FLAG);

		_current_path = full_path;

		rtos::ThisThread::sleep_for(100ms);

		_video.clearScreen();
		_video.displayImage(file);

		file.close();
	}
}

void VideoKit::playVideoOnce(const std::filesystem::path &path, const std::function<void()> &on_video_ended_callback)
{
	const std::scoped_lock lock(mutex);

	auto full_path = _default_video_path / path;

	if (auto file = FileManagerKit::File {full_path}; file.is_open()) {
		file.close();

		_event_flags.set(flags::STOP_VIDEO_FLAG);

		_current_path = full_path;
		_must_loop	  = false;

		rtos::ThisThread::sleep_for(100ms);
		_event_flags.set(flags::START_VIDEO_FLAG);

		_on_video_ended_callback = on_video_ended_callback;
	}
}

void VideoKit::playVideoOnRepeat(const std::filesystem::path &path,
								 const std::function<void()> &on_video_ended_callback)
{
	const std::scoped_lock lock(mutex);

	auto full_path = _default_video_path / path;

	if (auto file = FileManagerKit::File {full_path}; file.is_open()) {
		file.close();

		_event_flags.set(flags::STOP_VIDEO_FLAG);

		_current_path = full_path;
		_must_loop	  = true;

		rtos::ThisThread::sleep_for(100ms);
		_event_flags.set(flags::START_VIDEO_FLAG);

		_on_video_ended_callback = on_video_ended_callback;
	}
}

void VideoKit::stopVideo()
{
	_event_flags.set(flags::STOP_VIDEO_FLAG);
}

void VideoKit::run()
{
	auto file = FileManagerKit::File {};

	auto keep_running = [this] {
		auto must_not_stop	  = !((_event_flags.get() & flags::STOP_VIDEO_FLAG) == flags::STOP_VIDEO_FLAG);
		auto is_still_playing = !_video.isLastFrame();

		return must_not_stop && (_must_loop || is_still_playing);
	};

	while (true) {
		_event_flags.wait_any(flags::START_VIDEO_FLAG);
		_event_flags.clear(flags::STOP_VIDEO_FLAG);

		file.open(_current_path);
		_video.setVideo(file);

		while (keep_running()) {
			_video.displayNextFrameVideo(file);

			rtos::ThisThread::sleep_for(1ms);
		}

		file.close();
		if (_on_video_ended_callback != nullptr) {
			_on_video_ended_callback();
		}
	}
}
