# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_spike(
	TARGET
		lk_command_kit

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
		CoreSPI
		CoreTimeout
		BehaviorKit
		CommandKit
		EventLoopKit
		IMUKit
		LedKit
		MotionKit
		VideoKit
)
