# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_ticker_timeout)

target_include_directories(spike_lk_ticker_timeout
	PRIVATE
	.
)

target_sources(spike_lk_ticker_timeout
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_ticker_timeout
	CoreTicker
	CoreTimeout
)

target_link_custom_leka_targets(spike_lk_ticker_timeout)
