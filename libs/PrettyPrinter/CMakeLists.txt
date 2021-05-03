# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(lib_PrettyPrinter STATIC)

target_include_directories(lib_PrettyPrinter
	PUBLIC
		./include
)

target_sources(lib_PrettyPrinter
	PRIVATE
		./source/PrettyPrinter.cpp
)

target_link_libraries(lib_PrettyPrinter
	PRIVATE
		mbed-os
		LogKit
)
