# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

shopt -s xpg_echo

BASE_DIR=$1
HEAD_DIR=$2

source ./.github/actions/compare_base_head_changes/utils.sh

#
# MARK: - bootloader statistics
#

echo "Creating statistics for bootloader"

bootloader_target_name="bootloader"
bootloader_memory_section_size="0x40000"

createSizeTextFile $BASE_DIR $bootloader_target_name
createSizeTextFile $HEAD_DIR $bootloader_target_name

# MARK: Flash Used
bootloader_base_flash_used="$(getUsedFlashSize $BASE_DIR $bootloader_target_name)"
bootloader_base_flash_used_percentage="$((100 * $bootloader_base_flash_used / $bootloader_memory_section_size))%"

bootloader_head_flash_used="$(getUsedFlashSize $HEAD_DIR $bootloader_target_name)"
bootloader_head_flash_used_percentage="$((100 * $bootloader_head_flash_used / $bootloader_memory_section_size))%"

output_bootloader_base_flash_used="$bootloader_base_flash_used&nbsp;($bootloader_base_flash_used_percentage)"
output_bootloader_head_flash_used="$bootloader_head_flash_used&nbsp;($bootloader_head_flash_used_percentage)"

OUTPUT_BOOTLOADER_FLASH_USED="$output_bootloader_base_flash_used<br>$output_bootloader_head_flash_used"

# MARK: Flash Used Delta
bootloader_diff_flash=$(($bootloader_head_flash_used - $bootloader_base_flash_used))
bootloader_diff_flash_percentage="$((100 * ($bootloader_head_flash_used - $bootloader_base_flash_used) / $bootloader_base_flash_used))%"

OUTPUT_BOOTLOADER_FLASH_USED_DELTA=""

if [ $bootloader_diff_flash -lt 0 ]; then
	OUTPUT_BOOTLOADER_FLASH_USED_DELTA=":chart_with_downwards_trend:<br>$bootloader_diff_flash&nbsp;($bootloader_diff_flash_percentage)"
elif [ $bootloader_diff_flash -gt 0 ]; then
	OUTPUT_BOOTLOADER_FLASH_USED_DELTA=":chart_with_upwards_trend:<br>+$bootloader_diff_flash&nbsp;(+$bootloader_diff_flash_percentage)"
else
	OUTPUT_BOOTLOADER_FLASH_USED="$output_bootloader_base_flash_used"
	OUTPUT_BOOTLOADER_FLASH_USED_DELTA="ø"
fi

# MARK: Flash Available
bootloader_base_flash_available="$(($bootloader_memory_section_size - $bootloader_base_flash_used))"
bootloader_base_flash_available_percentage="$((100 * $bootloader_base_flash_available / $bootloader_memory_section_size))%"

bootloader_head_flash_available="$(($bootloader_memory_section_size - $bootloader_head_flash_used))"
bootloader_head_flash_available_percentage="$((100 * $bootloader_head_flash_available / $bootloader_memory_section_size))%"

output_bootloader_base_flash_available="$bootloader_base_flash_available&nbsp;($bootloader_base_flash_available_percentage)"
output_bootloader_head_flash_available="$bootloader_head_flash_available&nbsp;($bootloader_head_flash_available_percentage)"

OUTPUT_BOOTLOADER_FLASH_AVAILABLE="$output_bootloader_base_flash_available<br>$output_bootloader_head_flash_available"

# MARK: Static RAM
output_bootloader_base_ram_with_percentage="$(getUsedRamSizeWithPercentage $BASE_DIR $bootloader_target_name)"
output_bootloader_head_ram_with_percentage="$(getUsedRamSizeWithPercentage $HEAD_DIR $bootloader_target_name)"

OUTPUT_BOOTLOADER_RAM="$output_bootloader_base_ram_with_percentage<br>$output_bootloader_head_ram_with_percentage"

# MARK: Static RAM Delta

bootloader_base_ram="$(getUsedRamSize $BASE_DIR $bootloader_target_name)"
bootloader_head_ram="$(getUsedRamSize $HEAD_DIR $bootloader_target_name)"

bootloader_diff_ram=$(($bootloader_head_ram - $bootloader_base_ram))
bootloader_diff_ram_percentage="$((100 * ($bootloader_head_ram - $bootloader_base_ram) / $bootloader_base_ram))%"

OUTPUT_BOOTLOADER_RAM_DELTA=""

if [ $bootloader_diff_ram -lt 0 ]; then
	OUTPUT_BOOTLOADER_RAM_DELTA=":chart_with_downwards_trend:<br>$bootloader_diff_ram&nbsp;($bootloader_diff_ram_percentage)"
elif [ $bootloader_diff_ram -gt 0 ]; then
	OUTPUT_BOOTLOADER_RAM_DELTA=":chart_with_upwards_trend:<br>+$bootloader_diff_ram&nbsp;(+$bootloader_diff_ram_percentage)"
else
	OUTPUT_BOOTLOADER_RAM="$output_bootloader_base_ram_with_percentage"
	OUTPUT_BOOTLOADER_RAM_DELTA="ø"
fi

#
# MARK: - os statistics
#

echo "Creating statistics for os"

leka_os_target_name="LekaOS"
leka_os_memory_section_size="0x17E000"

createSizeTextFile $BASE_DIR $leka_os_target_name
createSizeTextFile $HEAD_DIR $leka_os_target_name

# MARK: Flash Used
leka_os_base_flash_used="$(getUsedFlashSize $BASE_DIR $leka_os_target_name)"
leka_os_base_flash_used_percentage="$((100 * $leka_os_base_flash_used / $leka_os_memory_section_size))%"

leka_os_head_flash_used="$(getUsedFlashSize $HEAD_DIR $leka_os_target_name)"
leka_os_head_flash_used_percentage="$((100 * $leka_os_head_flash_used / $leka_os_memory_section_size))%"

output_leka_os_base_flash_used="$leka_os_base_flash_used&nbsp;($leka_os_base_flash_used_percentage)"
output_leka_os_head_flash_used="$leka_os_head_flash_used&nbsp;($leka_os_head_flash_used_percentage)"

OUTPUT_LEKA_OS_FLASH_USED="$output_leka_os_base_flash_used<br>$output_leka_os_head_flash_used"

# MARK: Flash Used Delta
leka_os_diff_flash=$(($leka_os_head_flash_used - $leka_os_base_flash_used))
leka_os_diff_flash_percentage="$((100 * ($leka_os_head_flash_used - $leka_os_base_flash_used) / $leka_os_base_flash_used))%"

OUTPUT_LEKA_OS_FLASH_USED_DELTA=""

if [ $leka_os_diff_flash -lt 0 ]; then
	OUTPUT_LEKA_OS_FLASH_USED_DELTA=":chart_with_downwards_trend:<br>$leka_os_diff_flash&nbsp;($leka_os_diff_flash_percentage)"
elif [ $leka_os_diff_flash -gt 0 ]; then
	OUTPUT_LEKA_OS_FLASH_USED_DELTA=":chart_with_upwards_trend:<br>+$leka_os_diff_flash&nbsp;(+$leka_os_diff_flash_percentage)"
else
	OUTPUT_LEKA_OS_FLASH_USED="$output_leka_os_base_flash_used"
	OUTPUT_LEKA_OS_FLASH_USED_DELTA="ø"
fi

# MARK: Flash Available
leka_os_base_flash_available="$(($leka_os_memory_section_size - $leka_os_base_flash_used))"
leka_os_base_flash_available_percentage="$((100 * $leka_os_base_flash_available / $leka_os_memory_section_size))%"

leka_os_head_flash_available="$(($leka_os_memory_section_size - $leka_os_head_flash_used))"
leka_os_head_flash_available_percentage="$((100 * $leka_os_head_flash_available / $leka_os_memory_section_size))%"

output_leka_os_base_flash_available="$leka_os_base_flash_available&nbsp;($leka_os_base_flash_available_percentage)"
output_leka_os_head_flash_available="$leka_os_head_flash_available&nbsp;($leka_os_head_flash_available_percentage)"

OUTPUT_LEKA_OS_FLASH_AVAILABLE="$output_leka_os_base_flash_available<br>$output_leka_os_head_flash_available"

# MARK: Static RAM
output_leka_os_base_ram_with_percentage="$(getUsedRamSizeWithPercentage $BASE_DIR $leka_os_target_name)"
output_leka_os_head_ram_with_percentage="$(getUsedRamSizeWithPercentage $HEAD_DIR $leka_os_target_name)"

OUTPUT_LEKA_OS_RAM="$output_leka_os_base_ram_with_percentage<br>$output_leka_os_head_ram_with_percentage"

# MARK: Static RAM Delta
leka_os_base_ram="$(getUsedRamSize $BASE_DIR $leka_os_target_name)"
leka_os_head_ram="$(getUsedRamSize $HEAD_DIR $leka_os_target_name)"

leka_os_diff_ram=$(($leka_os_head_ram - $leka_os_base_ram))
leka_os_diff_ram_percentage="$((100 * ($leka_os_head_ram - $leka_os_base_ram) / $leka_os_base_ram))%"

OUTPUT_LEKA_OS_RAM_DELTA=""

if [ $leka_os_diff_ram -lt 0 ]; then
	OUTPUT_LEKA_OS_RAM_DELTA=":chart_with_downwards_trend:<br>$leka_os_diff_ram&nbsp;($leka_os_diff_ram_percentage)"
elif [ $leka_os_diff_ram -gt 0 ]; then
	OUTPUT_LEKA_OS_RAM_DELTA=":chart_with_upwards_trend:<br>+$leka_os_diff_ram&nbsp;(+$leka_os_diff_ram_percentage)"
else
	OUTPUT_LEKA_OS_RAM="$output_leka_os_base_ram_with_percentage"
	OUTPUT_LEKA_OS_RAM_DELTA="ø"
fi

#
# MARK: - markdown output
#

echo "Creating markdown output"

echo 'FIRMWARE_STATISTICS_OUTPUT<<EOF_FIRMWARE_STATISTICS_OUTPUT' >> $GITHUB_ENV

echo -n "| Target | Flash Used (base/head) | Fash Used Δ | Flash Available (base/head) | Static RAM (base/head) | Static RAM Δ |\n" >> $GITHUB_ENV
echo -n "|--------|:----------------------:|:-----------:|:---------------------------:|:----------------------:|:------------:|\n" >> $GITHUB_ENV

echo -n "| bootloader | $OUTPUT_BOOTLOADER_FLASH_USED | $OUTPUT_BOOTLOADER_FLASH_USED_DELTA | $OUTPUT_BOOTLOADER_FLASH_AVAILABLE | $OUTPUT_BOOTLOADER_RAM | $OUTPUT_BOOTLOADER_RAM_DELTA |\n" >> $GITHUB_ENV
echo -n "| os         | $OUTPUT_LEKA_OS_FLASH_USED    | $OUTPUT_LEKA_OS_FLASH_USED_DELTA    | $OUTPUT_LEKA_OS_FLASH_AVAILABLE    | $OUTPUT_LEKA_OS_RAM    | $OUTPUT_LEKA_OS_RAM_DELTA    |\n" >> $GITHUB_ENV

echo 'EOF_FIRMWARE_STATISTICS_OUTPUT' >> $GITHUB_ENV
