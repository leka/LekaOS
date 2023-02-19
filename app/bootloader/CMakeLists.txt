# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(bootloader)

target_include_directories(bootloader
	PRIVATE
	.
)

target_sources(bootloader
	PRIVATE
	main.cpp
	default_bd.cpp
	signing_keys.c
)

target_link_libraries(bootloader
	bootutil
	CoreBattery
	FileManagerKit
	CoreQSPI
	CoreFlashMemory
	FirmwareKit
	ConfigKit
	CoreLED
	CoreMotor
	CorePwm
)
