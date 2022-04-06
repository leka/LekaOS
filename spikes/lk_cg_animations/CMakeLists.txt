# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_cg_animations)

target_include_directories(spike_lk_cg_animations
	PRIVATE
		.
)

target_sources(spike_lk_cg_animations
	PRIVATE
		main.cpp
)

target_link_libraries(spike_lk_cg_animations
	CoreVideo
	CoreLL
	CoreSTM32Hal
	FileManagerKit
	UIAnimationKit
)

target_link_custom_leka_targets(spike_lk_cg_animations)
