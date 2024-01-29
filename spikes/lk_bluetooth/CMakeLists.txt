# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_spike(
	TARGET
		lk_bluetooth

	INCLUDE_DIRECTORIES
		.

	SOURCES
		main.cpp
		BM64Converter.cpp
		LekaBluetooth.cpp

	LINK_LIBRARIES # n/a
)
