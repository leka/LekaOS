# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

shopt -s xpg_echo

#
# MARK: - Find all targets
#

base_targets=($(echo $(find base_ref/_build -name '*.bin' -execdir basename -s '.bin' {} +) | tr ' ' '\n' | sort -du | tr '\n' ' '))
head_targets=($(echo $(find head_ref/_build -name '*.bin' -execdir basename -s '.bin' {} +) | tr ' ' '\n' | sort -du | tr '\n' ' '))
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
	local REF=$1
    local TARGET_NAME=$2
	python3 ./extern/mbed-os/tools/memap.py -t GCC_ARM $REF/_build/$TARGET_NAME.map > $REF/_build/$TARGET_NAME-map.txt
}

function createSizeTextFile() {
	local REF=$1
    local TARGET_NAME=$2
	bash ./tools/get_code_size.sh $REF/_build/$TARGET_NAME.elf --markdown > $REF/_build/$TARGET_NAME-code_size.txt
}

function getUsedFlashSize() {
	local REF=$1
    local TARGET_NAME=$2

	size=$(grep -Po '(?<=Flash used:\s)[[:digit:]]*' $REF/_build/$TARGET_NAME-code_size.txt)
	echo $size
}


function getUsedFlashSizeWithPercentage() {
	local REF=$1
    local TARGET_NAME=$2

	size=$(grep -Po '(?<=Flash used:\s)[[:digit:]]*&nbsp;\([[:digit:]]*%\)' $REF/_build/$TARGET_NAME-code_size.txt)
	echo $size
}

function getUsedRamSize() {
	local REF=$1
    local TARGET_NAME=$2

	size=$(grep -Po '(?<=SRAM used:\s)[[:digit:]]*' $REF/_build/$TARGET_NAME-code_size.txt)
	echo $size
}


function getUsedRamSizeWithPercentage() {
	local REF=$1
    local TARGET_NAME=$2

	size=$(grep -Po '(?<=SRAM used:\s)[[:digit:]]*&nbsp;\([[:digit:]]*%\)' $REF/_build/$TARGET_NAME-code_size.txt)
	echo $size
}
