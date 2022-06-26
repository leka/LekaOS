# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_rfid)

target_include_directories(spike_lk_rfid
	PRIVATE
	.
)

target_sources(spike_lk_rfid
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_rfid
	CoreRFIDReader
	RFIDKit
	CoreBufferedSerial
)

target_link_custom_leka_targets(spike_lk_rfid)
