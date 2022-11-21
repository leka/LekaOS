# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(IMUKit STATIC)

target_include_directories(IMUKit
	PUBLIC
	include
)

target_sources(IMUKit
	PRIVATE
	include/internal/Mahony.cpp
)

target_link_libraries(IMUKit
)
