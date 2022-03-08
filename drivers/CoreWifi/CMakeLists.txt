# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreWifi STATIC)

target_include_directories(CoreWifi
	PUBLIC
		include
		include/internal
)

target_sources(CoreWifi
	PRIVATE
		source/CoreWifi.cpp
		source/CoreNetwork.cpp
)

target_link_libraries(CoreWifi
	mbed-os
	mbed-http
)
