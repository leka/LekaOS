# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_imu_game)

target_include_directories(spike_lk_imu_game
	PRIVATE
)

target_sources(spike_lk_imu_game
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_imu_game
	CoreRFIDReader
	RFIDKit
	CoreBufferedSerial
	CoreLED
	ColorKit
	LedKit
	CoreEventFlags
	CoreVideo
	CoreLL
	CoreSTM32Hal
	CoreEventFlags
	FileManagerKit
	VideoKit
	CoreSPI
	CoreIMULSM6DSOX
	MotionKit
)

target_link_custom_leka_targets(spike_lk_imu_game)
