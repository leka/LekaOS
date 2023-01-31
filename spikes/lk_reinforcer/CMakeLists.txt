# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_reinforcer)

target_include_directories(spike_lk_reinforcer
	PRIVATE
	.
)

target_sources(spike_lk_reinforcer
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_reinforcer
	VideoKit
	CoreLED
	LedKit
	CoreMotor
	CorePwm
	CoreI2C
	CoreIMU
	EventLoopKit
	IMUKit
	MotionKit
	ReinforcerKit
	CoreTimeout
)

target_link_custom_leka_targets(spike_lk_reinforcer)
