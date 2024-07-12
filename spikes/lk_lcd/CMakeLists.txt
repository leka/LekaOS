# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_spike(
	TARGET
		lk_lcd

	INCLUDE_DIRECTORIES
		.

	SOURCES
		main.cpp

	LINK_LIBRARIES
		CoreLL
		CorePwm
		CoreSTM32Hal
		CoreVideo
		EventLoopKit
		FileManagerKit
		VideoKit
)
