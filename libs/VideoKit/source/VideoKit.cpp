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

	_event_loop.registerCallback([this] { run(); });
}

void VideoKit::displayImage(const std::filesystem::path &path)
{
	const std::scoped_lock lock(mutex);

	if (path == _current_path) {
		return;
	}

	if (auto file = FileManagerKit::File {path}; file.is_open()) {
		_must_stop = true;
		_event_loop.stop();

		_current_path = path;

		rtos::ThisThread::sleep_for(100ms);

		_video.displayImage(file);

		file.close();
	}
}

void VideoKit::fillWhiteBackgroundAndDisplayImage(const std::filesystem::path &path)
{
	const std::scoped_lock lock(mutex);

	if (path == _current_path) {
		return;
	}

	if (auto file = FileManagerKit::File {path}; file.is_open()) {
		_must_stop = true;
		_event_loop.stop();

		_current_path = path;

		rtos::ThisThread::sleep_for(100ms);

		_video.clearScreen();
		_video.displayImage(file);

		file.close();
	}
}

void VideoKit::playVideoOnce(const std::filesystem::path &path, const std::function<void()> &on_video_ended_callback)
{
	const std::scoped_lock lock(mutex);

	if (auto file = FileManagerKit::File {path}; file.is_open()) {
		file.close();

		_must_stop = true;
		_event_loop.stop();

		_current_path = path;
		_must_loop	  = false;

		rtos::ThisThread::sleep_for(100ms);

		_on_video_ended_callback = on_video_ended_callback;
		_event_loop.start();
	}
}

void VideoKit::playVideoOnRepeat(const std::filesystem::path &path,
								 const std::function<void()> &on_video_ended_callback)
{
	const std::scoped_lock lock(mutex);

	if (auto file = FileManagerKit::File {path}; file.is_open()) {
		file.close();

		_must_stop = true;
		_event_loop.stop();

		_current_path = path;
		_must_loop	  = true;

		rtos::ThisThread::sleep_for(100ms);

		_on_video_ended_callback = on_video_ended_callback;
		_event_loop.start();
	}
}

void VideoKit::stopVideo()
{
	_must_stop = true;
}

void VideoKit::run()
{
	auto keep_running = [this] {
		auto must_not_stop	  = !_must_stop;
		auto is_still_playing = !_video.isLastFrame();

		return must_not_stop && (_must_loop || is_still_playing);
	};

	_must_stop = false;

	auto file = FileManagerKit::File {_current_path};
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
