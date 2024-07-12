# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_spike(
	TARGET
		lk_cg_animations

	INCLUDE_DIRECTORIES
		.

	SOURCES
		main.cpp

	LINK_LIBRARIES
		CoreLL
		CorePwm
		CoreSTM32Hal
		CoreVideo
		FileManagerKit
		UIAnimationKit
)
