# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(mocks STATIC)

target_include_directories(mocks
	PUBLIC
	.
)

target_sources(mocks
	PUBLIC
	mocks/leka/source/mocks.cpp
)
