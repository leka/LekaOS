# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(MagicCardKit STATIC)

target_include_directories(MagicCardKit
	PUBLIC
	include
	include/activity
)

target_sources(MagicCardKit
	PRIVATE
	source/MagicCardKit.cpp
	source/StateMachineActivity.cpp
	source/activity/SelectReinforcers.cpp
)

target_link_libraries(MagicCardKit
	mbed-os
	CoreRFIDReader
	RFIDKit
	LedKit
	BehaviorKit
)

