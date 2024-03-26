# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_command_kit)

target_include_directories(spike_lk_command_kit
	PRIVATE
	.
)

target_sources(spike_lk_command_kit
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_command_kit
	CoreLED
	CoreMotor
	CorePwm
	CoreSPI
	CommandKit
	LedKit
	VideoKit
	CoreI2C
	CoreIMU
	IMUKit
	MotionKit
	CoreTimeout
	EventLoopKit
	CoreDAC
	AudioKit
	CoreQSPI
	CoreFlashMemory
)

target_link_custom_leka_targets(spike_lk_command_kit)
