# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(functional_ut_qdac)

target_include_directories(functional_ut_qdac
	PRIVATE
	${TESTS_FUNCTIONAL_INCLUDE_DIR}
)

target_sources(functional_ut_qdac
	PRIVATE
	suite_qdac.cpp
)

target_link_libraries(functional_ut_qdac
	CoreI2C
	CoreQDAC
)

add_main_to_functional_test(functional_ut_qdac)

target_link_custom_leka_targets(functional_ut_qdac)
