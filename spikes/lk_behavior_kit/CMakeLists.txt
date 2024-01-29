# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_spike(
	TARGET
		lk_behavior_kit

	INCLUDE_DIRECTORIES
		.

	SOURCES
		main.cpp

	LINK_LIBRARIES
		mbed-storage-fat
		mbed-storage-sd
		CoreMotor
		CorePwm
		CoreSPI
		CoreSTM32Hal
		CoreVideo
		BehaviorKit
		EventLoopKit
		LedKit
		VideoKit
)
