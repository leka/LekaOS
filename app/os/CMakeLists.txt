# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(LekaOS)

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
	RobotKit
	CoreMCU
	CoreTimeout
	CoreBattery
	CoreQSPI
	CoreFlashMemory
	FileManagerKit
	SerialNumberKit
	FirmwareKit
	BehaviorKit
	CorePwm
	CoreMotor
	VideoKit
	LedKit
	CoreLED
	CoreEventFlags
	CoreBufferedSerial
	CoreRFIDReader
	RFIDKit
	ActivityKit
)

target_link_custom_leka_targets(LekaOS)
