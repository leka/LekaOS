# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_spike(
	TARGET
		lk_log_kit

	INCLUDE_DIRECTORIES
		.

	SOURCES
		main.cpp
		source/utils.cpp

	LINK_LIBRARIES
		CoreEventQueue
)
