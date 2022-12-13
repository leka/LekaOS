# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(mocks INTERFACE)

target_include_directories(mocks
	INTERFACE
	.
)

target_sources(mocks
	INTERFACE
	mocks/leka/source/mocks.cpp
)
