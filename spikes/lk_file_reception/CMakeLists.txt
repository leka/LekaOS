# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_lk_file_reception)

target_include_directories(spike_lk_file_reception
	PRIVATE
		.
)

target_sources(spike_lk_file_reception
	PRIVATE
		main.cpp
		WebKit.cpp
)

target_link_libraries(spike_lk_file_reception
	PUBLIC
		mbed-os-static
		FileSystemKit
		CoreWifi
		mbed-http
)

mbed_set_post_build(spike_lk_file_reception)
