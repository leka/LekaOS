// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "rtos/Thread.h"

#include "interface/drivers/EventFlags.h"
#include "interface/drivers/Video.h"
#include "interface/libs/VideoKit.h"

namespace leka {

class VideoKit : public interface::VideoKit
{
  public:
	explicit VideoKit(interface::EventFlags &event_flags, interface::Video &video)
		: _event_flags(event_flags), _video {video}
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

	[[noreturn]] void run();

	struct flags {
		static constexpr uint32_t START_VIDEO_FLAG = (1UL << 1);
		static constexpr uint32_t STOP_VIDEO_FLAG  = (1UL << 2);
	};

  private:
	rtos::Thread _thread {};
	interface::EventFlags &_event_flags;

	interface::Video &_video;

	std::filesystem::path _current_path {};
	std::function<void()> _on_video_ended_callback {};
	bool _must_loop {false};
};

}	// namespace leka
