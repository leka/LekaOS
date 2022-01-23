#!/bin/bash

# Variables
BOOTLOADER_HEX_SOURCE="_build/LEKA_V1_2_DEV/app/bootloader/bootloader.hex"
BOOTLOADER_HEX_DEST="_tmp/bootloader.hex"

# Create dir
mkdir -p _tmp

# Compile bootloader
make deep_clean
make config
make bootloader

# Get bootloader binary
cp $BOOTLOADER_HEX_SOURCE $BOOTLOADER_HEX_DEST
