# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_core(CoreWifi
	PUBLIC_INCLUDE_DIRS
		include
		include/internal

	PRIVATE_SOURCES
		source/CoreWifi.cpp
		source/CoreNetwork.cpp

	PRIVATE_LINK_LIBRARIES
		mbed-wifi
		mbed-http
)
