#!/bin/bash

# Variables
BOOTLOADER_HEX_SOURCE="_build/LEKA_V1_2_DEV/app/bootloader/bootloader.hex"
BOOTLOADER_HEX_DESTINATION="$1"

if [ -z "$BOOTLOADER_HEX_DESTINATION" ]; then
	echo "APPLICATION_HEX_SOURCE is unset"
	exit 1
fi

# Compile bootloader
make deep_clean
make config
make bootloader

# Get bootloader binary
cp $BOOTLOADER_HEX_SOURCE $BOOTLOADER_HEX_DESTINATION
