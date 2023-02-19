# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_lk_update_process_app_base)

target_include_directories(spike_lk_update_process_app_base
	PRIVATE
	.
)

target_sources(spike_lk_update_process_app_base
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_update_process_app_base
	bootutil
	CoreQSPI
	CoreFlashMemory
	FileManagerKit
	FirmwareKit
)
