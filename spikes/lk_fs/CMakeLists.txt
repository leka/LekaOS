# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_spike(
	TARGET
		lk_fs

	INCLUDE_DIRECTORIES
		.
		include

	SOURCES
		main.cpp
		source/ComUtils.cpp

	LINK_LIBRARIES
		CoreEventQueue
		CorePwm
		CoreVideo
		EventLoopKit
		VideoKit
)
