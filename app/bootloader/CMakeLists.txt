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
	mbed-os
	mbed-storage-qspif

	CoreBattery
	CoreFlashMemory
	CoreLED
	CoreMotor
	CorePwm
	CoreQSPI
	CoreSPI
	ConfigKit
	FileManagerKit
	FirmwareKit
)

mbed_set_post_build(bootloader)
