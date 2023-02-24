# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

shopt -s xpg_echo

BASE_DIR=$1
HEAD_DIR=$2

#
# MARK: - Find all targets
#

echo "Set all targets"

base_targets=($(echo $(find $BASE_DIR -name '*.bin' -execdir basename -s '.bin' {} +) | tr ' ' '\n' | sort -du | tr '\n' ' '))
head_targets=($(echo $(find $HEAD_DIR -name '*.bin' -execdir basename -s '.bin' {} +) | tr ' ' '\n' | sort -du | tr '\n' ' '))
all_targets=($(echo "${base_targets[@]} ${head_targets[@]}" | tr ' ' '\n' | sort -du | tr '\n' ' '))

#
# MARK: - Find added/deleted targets
#

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

#
# MARK: - Echo results
#

echo "all:     ${all_targets[*]}"
echo "base:    ${base_targets[*]}"
echo "head:    ${head_targets[*]}"
echo "added:   ${added_targets[*]}"
echo "deleted: ${deleted_targets[*]}"
