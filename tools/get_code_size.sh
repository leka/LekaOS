#!/bin/bash

# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0
# Inspired by https://interrupt.memfault.com/blog/best-firmware-size-tools

ELF_FILE=$1
MARKDOWN_OUTPUT=$2

FLASH_SIZE=0x200000 # STM32F769 Flash size 2048 KiB
SRAM_SIZE=0x80000    # STM32F769 SRAM size 512 KiB

function print_region() {
	name=$1
    used_size=$2
    region_size=$3

    if [[ $region_size == 0x* ]]; then
        region_size=$(echo ${region_size:2})
        region_size=$(( 16#$region_size ))
    fi

    percentage=$(( 100 * $used_size / $region_size ))

	if [ -z "$MARKDOWN_OUTPUT" ]; then
		echo "$name used: $used_size ($percentage%) / total: $region_size"
	else
		echo "$name used: $used_size&nbsp;($percentage%) / total: $region_size"
	fi
}

raw=$(arm-none-eabi-size $ELF_FILE)

text=$(echo $raw | cut -d ' ' -f 7)
data=$(echo $raw | cut -d ' ' -f 8)
bss=$(echo $raw | cut -d ' ' -f 9)

flash=$(($text + $data))
ram=$(($data + $bss))

print_region "Flash" $flash $FLASH_SIZE
print_region "SRAM" $ram $SRAM_SIZE
