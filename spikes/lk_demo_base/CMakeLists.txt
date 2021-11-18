# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_demo_base)

target_include_directories(spike_lk_demo_base
	PRIVATE
		.
)

target_sources(spike_lk_demo_base
	PRIVATE
		main.cpp
		RFIDUtils.cpp
		DisplayUtils.cpp
		BatteryUtils.cpp
		WatchdogUtils.cpp
)

target_link_libraries(spike_lk_demo_base
	CoreBufferedSerial
	FileSystemKit
	LKCoreSTM32Hal
	CoreBattery
	CoreVideo
	VideoKit
	CoreCR95HF
	RFIDKit
)

target_link_custom_leka_targets(spike_lk_demo_base)
