# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_lk_fs)

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
	CorePwm
	CoreVideo
	EventLoopKit
	VideoKit
)
