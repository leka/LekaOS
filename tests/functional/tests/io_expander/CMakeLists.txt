# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(functional_ut_io_expander)

target_include_directories(functional_ut_io_expander
	PRIVATE
	${TESTS_FUNCTIONAL_INCLUDE_DIR}
)

target_sources(functional_ut_io_expander
	PRIVATE
	suite_io_expander.cpp
)

target_link_libraries(functional_ut_io_expander
	CoreI2C
	CoreIOExpander
)

add_main_to_functional_test(functional_ut_io_expander)

target_link_custom_leka_targets(functional_ut_io_expander)
