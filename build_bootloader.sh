#!/bin/bash

mkdir _tmp > /dev/null 2> /dev/null

# Variables
BOOTLOADER_HEX_SOURCE="$1"
BOOTLOADER_HEX="$2"

if [ -z "$BOOTLOADER_HEX_SOURCE" ]; then
	echo "BOOTLOADER_HEX_SOURCE is unset"
	BOOTLOADER_HEX_SOURCE="_build/LEKA_V1_2_DEV/app/bootloader/bootloader.hex"
fi

if [ -z "$BOOTLOADER_HEX" ]; then
	echo "BOOTLOADER_HEX is unset"
	BOOTLOADER_HEX="_tmp/bootloader.hex"
fi

# Compile bootloader
make deep_clean
make config
make

# Get bootloader binary
cp $BOOTLOADER_HEX_SOURCE $BOOTLOADER_HEX
