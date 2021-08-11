# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_video_thread)

target_include_directories(spike_lk_video_thread
	PRIVATE
		.
)

target_sources(spike_lk_video_thread
	PRIVATE
		main.cpp
)

target_link_libraries(spike_lk_video_thread
	lib_LekaScreen
	CoreFs
	CoreVideo
	LKCoreLL
	LKCoreSTM32Hal
	VideoKit
)

target_link_custom_leka_targets(spike_lk_video_thread)
