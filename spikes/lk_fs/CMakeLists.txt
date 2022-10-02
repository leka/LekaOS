# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_fs)

target_include_directories(spike_lk_fs
	PRIVATE
	.
	include
)

target_sources(spike_lk_fs
	PRIVATE
	main.cpp
	source/ComUtils.cpp
)

target_link_libraries(spike_lk_fs
	CoreEventFlags
	CorePwm
	CoreVideo
	EventLoopKit
	VideoKit
)

target_link_custom_leka_targets(spike_lk_fs)
