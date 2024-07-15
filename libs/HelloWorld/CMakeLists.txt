# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(HelloWorld
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/HelloWorld.cpp

	PRIVATE_LINK_LIBRARIES
		mbed_headers
)
