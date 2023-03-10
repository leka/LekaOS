# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_activity_kit)

target_include_directories(spike_lk_activity_kit
	PRIVATE
	.
)

target_sources(spike_lk_activity_kit
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_activity_kit
	CoreTimeout
	FileManagerKit
	BehaviorKit
	CorePwm
	CoreMotor
	VideoKit
	LedKit
	CoreLED
	CoreBufferedSerial
	CoreRFIDReader
	RFIDKit
	ActivityKit
	CoreI2C
	CoreIMU
	IMUKit
	MotionKit
	EventLoopKit
)

target_link_custom_leka_targets(spike_lk_activity_kit)
