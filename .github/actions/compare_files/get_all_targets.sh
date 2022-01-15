# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

shopt -s xpg_echo

BASE_DIR=$1
HEAD_DIR=$2

#
# MARK: - Find all targets
#

base_targets=($(echo $(find $BASE_DIR -name '*.bin' -execdir basename -s '.bin' {} +) | tr ' ' '\n' | sort -du | tr '\n' ' '))
head_targets=($(echo $(find $HEAD_DIR -name '*.bin' -execdir basename -s '.bin' {} +) | tr ' ' '\n' | sort -du | tr '\n' ' '))
all_targets=($(echo "${base_targets[@]} ${head_targets[@]}" | tr ' ' '\n' | sort -du | tr '\n' ' '))

echo "Set added/deleted targets"

added_targets=()
deleted_targets=()

for target in "${all_targets[@]}"; do
	if [[ ${base_targets[*]} =~ "$target" ]] && ! [[ ${head_targets[*]} =~ "$target" ]]; then
		deleted_targets+=($target)
	elif ! [[ ${base_targets[*]} =~ "$target" ]] && [[ ${head_targets[*]} =~ "$target" ]]; then
		added_targets+=($target)
	fi
done

echo "all:     ${all_targets[*]}"
echo "base:    ${base_targets[*]}"
echo "head:    ${head_targets[*]}"
echo "added:   ${added_targets[*]}"
echo "deleted: ${deleted_targets[*]}"

#
# MARK: - Utils functions
#

function createMapTextFile() {
	local DIR=$1
	local TARGET_NAME=$2
	python3 ./extern/mbed-os/tools/memap.py -t GCC_ARM $DIR/$TARGET_NAME.map > $DIR/$TARGET_NAME-map.txt
}

function createSizeTextFile() {
	local DIR=$1
	local TARGET_NAME=$2
	bash ./tools/get_code_size.sh $DIR/$TARGET_NAME.elf --markdown > $DIR/$TARGET_NAME-code_size.txt
}

function getUsedFlashSize() {
	local DIR=$1
	local TARGET_NAME=$2

	size=$(grep -Po '(?<=Flash used:\s)[[:digit:]]*' $DIR/$TARGET_NAME-code_size.txt)
	echo $size
}


function getUsedFlashSizeWithPercentage() {
	local DIR=$1
	local TARGET_NAME=$2

	size=$(grep -Po '(?<=Flash used:\s)[[:digit:]]*&nbsp;\([[:digit:]]*%\)' $DIR/$TARGET_NAME-code_size.txt)
	echo $size
}

function getUsedRamSize() {
	local DIR=$1
	local TARGET_NAME=$2

	size=$(grep -Po '(?<=SRAM used:\s)[[:digit:]]*' $DIR/$TARGET_NAME-code_size.txt)
	echo $size
}


function getUsedRamSizeWithPercentage() {
	local DIR=$1
	local TARGET_NAME=$2

	size=$(grep -Po '(?<=SRAM used:\s)[[:digit:]]*&nbsp;\([[:digit:]]*%\)' $DIR/$TARGET_NAME-code_size.txt)
	echo $size
}
