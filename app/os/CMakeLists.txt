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
	CoreTimeout
	CoreBattery
	CoreQSPI
	CoreFlashMemory
	FileSystemKit
	FirmwareKit
	BehaviorKit
	CorePwm
	CoreMotor
	VideoKit
	LedKit
	CoreLED
)

target_link_custom_leka_targets(LekaOS)
