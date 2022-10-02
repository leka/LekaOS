# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(certs_emc_lcd_led_motors)

target_include_directories(certs_emc_lcd_led_motors
	PRIVATE
		.
		../../include
)

target_sources(certs_emc_lcd_led_motors
	PRIVATE
		main.cpp
)

target_link_libraries(certs_emc_lcd_led_motors
	CorePwm
	CoreMotor
	driver_FastLED
	lib_LekaScreen
)

target_link_custom_leka_targets(certs_emc_lcd_led_motors)
