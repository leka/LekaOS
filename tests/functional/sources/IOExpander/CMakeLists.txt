# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(suite_io_expander)

leka_functional_tests_includes(suite_io_expander
	.
)

leka_functional_tests_sources(suite_io_expander
	suite_io_expander.cpp
)

target_link_libraries(suite_io_expander
	CoreI2C
	CoreIOExpander
)

target_link_custom_leka_targets(suite_io_expander)
