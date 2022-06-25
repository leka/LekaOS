# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(HelloWorld STATIC)

target_include_directories(HelloWorld
	PUBLIC
	include
)

target_sources(HelloWorld
	PRIVATE
	source/HelloWorld.cpp
)

target_link_libraries(HelloWorld
	mbed-os
)
