# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

shopt -s xpg_echo

BASE_DIR=$1
HEAD_DIR=$2

source ./.github/actions/compare_files/utils.sh

#
# MARK: - bootloader statistics
#

echo "Creating statistics for bootloader"

bootloader_target_name="bootloader"
bootloader_memory_section_size="0x40000"

createSizeTextFile $HEAD_DIR $bootloader_target_name

bootloader_head_flash_used="$(getUsedFlashSize $HEAD_DIR $bootloader_target_name)"
bootloader_head_flash_used_percentage="$((100 * $bootloader_head_flash_used / $bootloader_memory_section_size))%"

output_bootloader_flash_used="$bootloader_head_flash_used&nbsp;($bootloader_head_flash_used_percentage)"

bootloader_head_flash_available="$(($bootloader_memory_section_size - $bootloader_head_flash_used))"
bootloader_head_flash_available_percentage="$((100 * $bootloader_head_flash_available / $bootloader_memory_section_size))%"

output_bootloader_flash_available="$bootloader_head_flash_available&nbsp;($bootloader_head_flash_available_percentage)"

output_booloader_ram_with_percentage="$(getUsedRamSizeWithPercentage $HEAD_DIR $bootloader_target_name)"

#
# MARK: - os statistics
#

echo "Creating statistics for os"

leka_os_target_name="LekaOS"
leka_os_memory_section_size="0x17E000"

createSizeTextFile $HEAD_DIR $leka_os_target_name

leka_os_head_flash_used="$(getUsedFlashSize $HEAD_DIR $leka_os_target_name)"
leka_os_head_flash_used_percentage="$((100 * $leka_os_head_flash_used / $leka_os_memory_section_size))%"

output_leka_os_flash_used="$leka_os_head_flash_used&nbsp;($leka_os_head_flash_used_percentage)"

leka_os_head_flash_available="$(($leka_os_memory_section_size - $leka_os_head_flash_used))"
leka_os_head_flash_available_percentage="$((100 * $leka_os_head_flash_available / $leka_os_memory_section_size))%"

output_leka_os_flash_available="$leka_os_head_flash_available&nbsp;($leka_os_head_flash_available_percentage)"

output_leka_os_ram_with_percentage="$(getUsedRamSizeWithPercentage $HEAD_DIR $leka_os_target_name)"

#
# MARK: - markdown output
#

echo "Creating markdown output"

echo 'FIRMWARE_STATISTICS_OUTPUT<<EOF_FIRMWARE_STATISTICS_OUTPUT' >> $GITHUB_ENV

echo -n "| Target | Flash Used (%) | Flash Available (%) | Static RAM (%) |\n" >> $GITHUB_ENV
echo -n "|--------|---------------:|--------------------:|---------------:|\n" >> $GITHUB_ENV

echo -n "| bootloader | $output_bootloader_flash_used | $output_bootloader_flash_available | $output_booloader_ram_with_percentage |\n" >> $GITHUB_ENV
echo -n "| os         | $output_leka_os_flash_used    | $output_leka_os_flash_available    | $output_leka_os_ram_with_percentage   |\n" >> $GITHUB_ENV

echo 'EOF_FIRMWARE_STATISTICS_OUTPUT' >> $GITHUB_ENV
