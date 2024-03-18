# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(AudioKit STATIC)

target_include_directories(AudioKit
	PUBLIC
	include
)

target_sources(AudioKit
	PRIVATE
	source/AudioKit.cpp
	source/WavFile.cpp
)

target_link_libraries(AudioKit
	mbed-os
	CoreSTM32Hal
	CoreDAC
	CoreEventQueue
	FileManagerKit
)
