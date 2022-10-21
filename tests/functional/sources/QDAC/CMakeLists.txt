# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(suite_qdac)

leka_functional_tests_includes(suite_qdac
	.
)

leka_functional_tests_sources(suite_qdac
	suite_qdac.cpp
)

target_link_libraries(suite_qdac
	CoreI2C
	CoreQDAC
)

target_link_custom_leka_targets(suite_qdac)
