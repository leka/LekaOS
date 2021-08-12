# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_audio_video)

target_include_directories(spike_lk_audio_video
	PRIVATE
		.
)

target_sources(spike_lk_audio_video
	PRIVATE
		main.cpp
)

target_link_libraries(spike_lk_audio_video
	LKCoreSTM32Hal
	CoreAudio
	CoreVideo
	VideoKit
	FileManager
)

target_link_custom_leka_targets(spike_lk_audio_video)