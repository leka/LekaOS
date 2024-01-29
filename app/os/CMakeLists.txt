# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(LekaOS)

target_include_directories(LekaOS
	PRIVATE
	.
)

target_sources(LekaOS
	PRIVATE
	main.cpp
)

target_link_libraries(LekaOS
	bootutil
	mbed-os

	CoreBattery
	CoreBufferedSerial
	CoreFlashMemory
	CoreI2C
	CoreIMU
	CoreLED
	CoreMCU
	CoreMotor
	CorePwm
	CoreQSPI
	CoreRFIDReader
	CoreSPI
	CoreTimeout

	ActivityKit
	BehaviorKit
	EventLoopKit
	FileManagerKit
	FirmwareKit
	IMUKit
	LedKit
	MotionKit
	RFIDKit
	ReinforcerKit
	RobotKit
	SerialNumberKit
	VideoKit
)

mbed_set_post_build(LekaOS)
