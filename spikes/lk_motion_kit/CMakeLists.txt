# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_spike(
	TARGET
		lk_motion_kit

	INCLUDE_DIRECTORIES
		.

	SOURCES
		main.cpp

	LINK_LIBRARIES
		CoreBufferedSerial
		CoreI2C
		CoreIMU
		CoreMotor
		CorePwm
		CoreRFIDReader
		CoreTimeout
		EventLoopKit
		IMUKit
		MotionKit
		RFIDKit
)
