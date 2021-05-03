# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(HTTP STATIC)

target_include_directories(HTTP
	PUBLIC
		http_parser
		source
)

target_sources(HTTP
	PRIVATE
		http_parser/http_parser.c
)

target_link_libraries(HTTP
		mbed-os
)

