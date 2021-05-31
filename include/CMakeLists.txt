# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(interface INTERFACE)

target_include_directories(interface
	INTERFACE
		.
		interface
		interface/drivers
)
