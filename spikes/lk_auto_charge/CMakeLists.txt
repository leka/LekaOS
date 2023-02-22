# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_auto_charge)

target_include_directories(spike_lk_auto_charge
	PRIVATE
	.
)

target_sources(spike_lk_auto_charge
	PRIVATE
	main.cpp
	SealStrategy.cpp
)

target_link_libraries(spike_lk_auto_charge
	EventLoopKit
	CoreTimeout
	CoreBattery
	BatteryKit
	CorePwm
	CoreMotor
	CoreIMU
	CoreI2C
	IMUKit
	BLEKit
)

target_link_custom_leka_targets(spike_lk_auto_charge)
