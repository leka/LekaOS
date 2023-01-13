# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_functional_test(
	TARGET
		functional_ut_io_expander

	INCLUDE_DIRECTORIES

	SOURCES
		suite_io_expander.cpp

	LINK_LIBRARIES
		CoreI2C
		CoreIOExpander
)
