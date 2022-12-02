# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_motion_kit)

target_include_directories(spike_lk_motion_kit
	PRIVATE
	.
)

target_sources(spike_lk_motion_kit
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_motion_kit
	CoreI2C
	CoreIMU
	CoreMotor
	CorePwm
	EventLoopKit
	IMUKit
	MotionKit
	CoreBufferedSerial
	CoreRFIDReader
	RFIDKit
)

target_link_custom_leka_targets(spike_lk_motion_kit)
