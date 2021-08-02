# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreAudio STATIC)

target_include_directories(CoreAudio
	PUBLIC
		include
		include/internal
)

target_sources(CoreAudio
	PRIVATE
		source/CoreDACTimer.cpp
		source/CoreDAC.cpp
)

target_link_libraries(CoreAudio
		mbed-os
		LKCoreSTM32Hal
)
