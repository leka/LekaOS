# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreAudio STATIC)

target_include_directories(CoreAudio
	PUBLIC
		include
		include/internal
)

target_sources(CoreAudio
	PRIVATE
		source/DACTimer.cpp
		source/DACDriver.cpp
		source/WavFile.cpp
		source/WavReader.cpp
		source/CoreAudio.cpp
)

target_link_libraries(CoreAudio
		mbed-os
		LKCoreSTM32Hal
		CoreFs
)
