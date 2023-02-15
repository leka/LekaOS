# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(Fusion STATIC)

target_include_directories(Fusion
	PUBLIC
	.
)

target_sources(Fusion
	PRIVATE
	fusion/FusionAhrs.c
	fusion/FusionCompass.c
	fusion/FusionOffset.c
)

if(UNIX AND NOT APPLE)
	target_link_libraries(Fusion m) # link math library for Linux
endif()
