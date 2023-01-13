# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_functional_test(
	TARGET
		functional_ut_file_manager

	INCLUDE_DIRECTORIES

	SOURCES
		suite_file_manager.cpp

	LINK_LIBRARIES
		FileManagerKit
)
