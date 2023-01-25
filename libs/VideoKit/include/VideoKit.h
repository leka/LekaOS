// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "rtos/Thread.h"

#include "interface/drivers/Video.h"
#include "interface/libs/EventLoop.h"
#include "interface/libs/VideoKit.h"

namespace leka {

class VideoKit : public interface::VideoKit
{
  public:
	explicit VideoKit(interface::EventLoop &event_loop, interface::Video &video)
		: _event_loop(event_loop), _video {video}
	{
		// nothing to do
	}

	void initializeScreen() final;

	void displayImage(const std::filesystem::path &path) final;
	void fillWhiteBackgroundAndDisplayImage(const std::filesystem::path &path) final;

	void playVideoOnce(const std::filesystem::path &path,
					   const std::function<void()> &on_video_ended_callback = {}) final;
	void playVideoOnRepeat(const std::filesystem::path &path,
						   const std::function<void()> &on_video_ended_callback = {}) final;

	void stopVideo() final;

	void run();

  private:
	interface::EventLoop &_event_loop;

	interface::Video &_video;

	std::filesystem::path _current_path {};
	std::function<void()> _on_video_ended_callback {};
	bool _must_stop {false};
	bool _must_loop {false};
};

}	// namespace leka
