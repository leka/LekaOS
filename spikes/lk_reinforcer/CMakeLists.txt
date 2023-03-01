# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_spike(
	TARGET
		lk_reinforcer

	INCLUDE_DIRECTORIES
		.

	SOURCES
		main.cpp

	LINK_LIBRARIES
		CoreI2C
		CoreIMU
		CoreLED
		CoreMotor
		CorePwm
		CoreTimeout
		EventLoopKit
		IMUKit
		LedKit
		MotionKit
		ReinforcerKit
	VideoKit
)
