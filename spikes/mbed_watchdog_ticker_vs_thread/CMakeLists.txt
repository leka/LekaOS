# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_mbed_watchdog_ticker_vs_thread)

target_include_directories(spike_mbed_watchdog_ticker_vs_thread
	PRIVATE
		.
)

target_sources(spike_mbed_watchdog_ticker_vs_thread
	PRIVATE
		main.cpp
)

target_link_custom_leka_targets(spike_mbed_watchdog_ticker_vs_thread)
